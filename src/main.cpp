#include <stdint.h>
#include <stdio.h>
#include <memory>
#include <queue>

#include "entity.hpp"
#include "event.hpp"
#include "map.hpp"
#include "gui.hpp"

struct Player : public Entity
{
  Player()
    : Entity(40, 25, '@')
  {
  }
};

struct EntityAction : public Event
{
  EntityAction(Entity *entity, int warmup_time, int cooldown_time)
    : Event(sim_cur_time()),
      entity_(entity),
      warmup_time_(warmup_time),
      cooldown_time_(cooldown_time),
      state_(STATE_INITIAL)
  {
    entity_->set_busy(true);
  }

  virtual void run()
  {
    switch (state_) {
      case STATE_INITIAL:
        state_ = STATE_WARMING_UP;
        reschedule(sim_cur_time() + warmup_time_);
        break;
      case STATE_WARMING_UP:
        action();
        state_ = STATE_COOLING_DOWN;
        reschedule(sim_cur_time() + cooldown_time_);
        break;
      case STATE_COOLING_DOWN:
        entity_->set_busy(false);
        break;
    }
  }

  virtual void action() = 0;

  Entity *entity_;
  int warmup_time_;
  int cooldown_time_;
  enum { STATE_INITIAL, STATE_WARMING_UP, STATE_COOLING_DOWN }
      state_;
};

struct EntityMoveAction : public EntityAction
{
  EntityMoveAction(Entity *entity, int dx, int dy, int move_time)
    : EntityAction(entity, 0, move_time),
      dx_(dx),
      dy_(dy)
  {
  }

  virtual void action()
  {
    entity_->move(dx_, dy_);
  }

  int dx_;
  int dy_;
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
    gui_msg("expjunk %p move by %d,%d", this, 
        exp_junk_->vel_x_, exp_junk_->vel_y_);
    exp_junk_->move(exp_junk_->vel_x_, exp_junk_->vel_y_);
    exp_junk_->lifetime_ -= TIME_DELTA;

    if (exp_junk_->lifetime_ > 0) {
      reschedule_ = true;
      time_ += TIME_DELTA;
    } else {
      // destroy junk?
    }

    gui_set_dirty();
  }

  ExplosionJunk::Ptr exp_junk_;
};

int main() 
{
  Map map;
  Player player;
  Gui gui;
  
  auto ej = std::make_shared<ExplosionJunk>(10, 10, 1, 1, 5000);

  sim_add_event(Event::Ptr(new ExplosionJunkUpdateEvent(ej)));

  /*
  TCODConsole::initRoot(80, 70, "libtcod test", false);
  TCODSystem::setFps(25);

  while (!TCODConsole::isWindowClosed()) {
    TCODConsole *console = TCODConsole::root;

    console->clear();
    map.render(console);
    gui.render(console);
    player.render(console);
    ej->render(console);

    TCODConsole::flush();
    gui_set_clean();

    while (player.is_busy() && !gui.is_dirty()) {
      sim_step();
    }

    if (!player.is_busy()) {
      TCOD_key_t key;
      TCOD_event_t event;

      event = TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, false);

      if (event == TCOD_EVENT_KEY_PRESS) {
        switch (key.vk) {
          case TCODK_UP:
            sim_add_event(Event::Ptr(new EntityMoveAction(&player, 0, -1, 100)));
            break;
          case TCODK_DOWN:
            sim_add_event(Event::Ptr(new EntityMoveAction(&player, 0, 1, 100)));
            break;
          case TCODK_LEFT:
            sim_add_event(Event::Ptr(new EntityMoveAction(&player, -1, 0, 100)));
            break;
          case TCODK_RIGHT:
            sim_add_event(Event::Ptr(new EntityMoveAction(&player, 1, 0, 100)));
            break;
          default:
            break;
        }
      }
    } else {
    }

  }
  */

  return 0;
}

