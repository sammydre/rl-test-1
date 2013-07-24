#include <stdint.h>
#include <stdio.h>
#include <memory>
#include <queue>

#include "libtcod.hpp"

struct Tile
{
  Tile()
    : ascii_('#'),
      bg_colour_(0xaa, 0xaa, 0xaa),
      fg_colour_(0xff, 0xff, 0xff)
  {
  }

  int ascii_;
  TCODColor bg_colour_;
  TCODColor fg_colour_;
};

struct Map
{
  Map()
    : width_(80),
      height_(50)
  {
    tiles_.reset(new Tile[width_ * height_]);
    setup();
  }

  void setup()
  {
    for (int y = 1; y < height_ - 1; y++) {
      for (int x = 1; x < width_ - 1; x++) {
        tile_at(x, y).ascii_ = '.';
        tile_at(x, y).bg_colour_ = TCODColor(0, 0, 0);
        tile_at(x, y).fg_colour_ = TCODColor(0x99, 0x99, 0x99);
      }
    }
  }

  void render(TCODConsole *con)
  {
    for (int y = 0; y < height_; y++) {
      for (int x = 0; x < width_; x++) {
        const Tile &t = tile_at(x, y);
        con->putChar(x, y, t.ascii_);
        con->setCharBackground(x, y, t.bg_colour_);
        con->setCharForeground(x, y, t.fg_colour_);
      }
    }
  }

  Tile &tile_at(int x, int y)
  {
    return tiles_[y * width_ + x];
  }

  int width_;
  int height_;
  std::unique_ptr<Tile[]> tiles_;
};

struct Entity
{
  Entity(int x, int y, int ascii)
    : x_(x),
      y_(y),
      ascii_(ascii)
  {
  }

  virtual void render(TCODConsole *con)
  {
    con->putChar(x_, y_, ascii_);
    con->setCharBackground(x_, y_, TCODColor(0, 0, 0));
    con->setCharForeground(x_, y_, TCODColor(0xff, 0xff, 0xff));
  }

  void move(int dx, int dy)
  {
    if (x_ + dx < 1 ||
        x_ + dx >= 79)
      return;
    if (y_ + dy < 1 ||
        y_ + dy >= 49)
      return;

    x_ += dx;
    y_ += dy;
  }

  int x_;
  int y_;
  int ascii_;
};

struct Player : public Entity
{
  Player()
    : Entity(40, 25, '@')
  {
  }
};

struct Event
{
  Event(int64_t time)
    : time_(time),
      cancelled_(false),
      reschedule_(false)
  {
  }
  
  virtual ~Event()
  {
  }

  bool operator<(const Event &rhs) const
  {
    // We reverse the comparison here to make the priority queue have
    // the lowest element at the top, not the greatest.
    return time_ > rhs.time_;
  }

  void cancel()
  {
    cancelled_ = true;
  }

  virtual void run() = 0;

  int64_t time_;
  bool cancelled_;
  bool reschedule_;

  typedef std::shared_ptr<Event> Ptr;
};

struct Simulator
{
  Simulator()
    : time_(0)
  {
  }

  void step()
  {
    if (queue_.empty())
      return;

    std::shared_ptr<Event> ev = queue_.top();
    queue_.pop();

    if (!ev->cancelled_) {
      fprintf(stderr, "Sim: run event at %ld\n", ev->time_);
      time_ = ev->time_;
      ev->run();
    }

    if (ev->reschedule_) {
      ev->cancelled_ = false;
      ev->reschedule_ = false;
      add(ev);
    }
  }

  void add(const std::shared_ptr<Event>& ev)
  {
    queue_.push(ev);
  }

  int64_t time_;
  std::priority_queue<std::shared_ptr<Event>> queue_;
};

void sim_add_event(const std::shared_ptr<Event>& ev)
{
  // TODO
}

int64_t sim_cur_time(void)
{
  return 0;
}

struct ExplosionJunk : public Entity
{
  ExplosionJunk(int x, int y, int vel_x, int vel_y, int lifetime)
    : Entity(x, y, '~'),
      vel_x_(vel_x),
      vel_y_(vel_y),
      lifetime_(lifetime)
  {
  }

  int vel_x_;
  int vel_y_;
  int lifetime_;

  typedef std::shared_ptr<ExplosionJunk> Ptr;
};

struct ExplosionJunkUpdateEvent : public Event
{
  enum {
    TIME_DELTA = 10
  };

  ExplosionJunkUpdateEvent(const ExplosionJunk::Ptr& j)
    : Event(sim_cur_time() + TIME_DELTA),
      exp_junk_(j)
  {
  }

  virtual void run()
  {
    fprintf(stderr, "move by %d,%d\n", exp_junk_->vel_x_, exp_junk_->vel_y_);
    exp_junk_->move(exp_junk_->vel_x_, exp_junk_->vel_y_);
    exp_junk_->lifetime_ -= TIME_DELTA;

    if (exp_junk_->lifetime_ > 0) {
      reschedule_ = true;
      time_ += TIME_DELTA;
    } else {
      // destroy junk?
    }
  }

  ExplosionJunk::Ptr exp_junk_;
};

int main() 
{
  Map map;
  Player player;
  Simulator simulator;
  
  ExplosionJunk::Ptr ej(new ExplosionJunk(10, 10, 1, 1, 5000));

  simulator.add(Event::Ptr(new ExplosionJunkUpdateEvent(ej)));

  TCODConsole::initRoot(80, 50, "libtcod test", false);

  while (!TCODConsole::isWindowClosed()) {
    TCODConsole *console = TCODConsole::root;

    console->clear();
    map.render(console);
    player.render(console);
    ej->render(console);

    TCODConsole::flush();

    TCOD_key_t key;
    TCOD_event_t event;

    event = TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, false);

    if (event == TCOD_EVENT_KEY_PRESS) {
      switch (key.vk) {
        case TCODK_UP:    player.move( 0, -1); break;
        case TCODK_DOWN:  player.move( 0,  1); break;
        case TCODK_LEFT:  player.move(-1,  0); break;
        case TCODK_RIGHT: player.move( 1,  0); break;
        default:
          break;
      }
    }

    simulator.step();
  }
  return 0;
}

