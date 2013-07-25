#include <stdint.h>
#include <stdio.h>
#include <memory>
#include <queue>

#include <libtcod.hpp>

#include "entity.hpp"
#include "event.hpp"
#include "map.hpp"

struct Player : public Entity
{
  Player()
    : Entity(40, 25, '@')
  {
  }
};

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
  
  ExplosionJunk::Ptr ej(new ExplosionJunk(10, 10, 1, 1, 5000));

  sim_add_event(Event::Ptr(new ExplosionJunkUpdateEvent(ej)));

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

    sim_step();
  }
  return 0;
}

