#include <stdint.h>
#include <stdio.h>
#include <memory>
#include <queue>

#include "entity.hpp"
#include "event.hpp"
#include "game.hpp"
#include "map.hpp"
#include "pc.hpp"


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


namespace {
  Map g_map;
  Player g_player;
}

Map *get_map() { return &g_map; }
Player *get_player() { return &g_player; }

struct PlayerAction
{
  virtual void perform() = 0;
};

struct PlayerMoveAction : PlayerAction
{
  PlayerMoveAction(int dx, int dy)
    : dx_(dx), dy_(dy)
  {}

  virtual void perform()
  {
    Map *m = get_map();
    Player *p = get_player();
    int mx = p->x_ + dx_,
        my = p->y_ + dy_;
    
    if (!m->contains(mx, my))
      return;

    Tile &t = m->tile_at(mx, my);

    if (!t.passable()) {
      // gui_message("You can't move there.");
      return;
    }
    
    p->set_busy(true);
    sim_add_event(Event::Ptr(new EntityMoveAction(p, dx_, dy_, 100)));
  }

private:
  int dx_, dy_;
};

void perform_action(ActionEnum e)
{
  std::unique_ptr<PlayerAction> act;

  switch (e) {
    case ActionMoveLeft:
      act.reset(new PlayerMoveAction(-1, +0)); break;
    case ActionMoveRight:
      act.reset(new PlayerMoveAction(+1, +0)); break;
    case ActionMoveUp:
      act.reset(new PlayerMoveAction(+0, -1)); break;
    case ActionMoveDown:
      act.reset(new PlayerMoveAction(+0, +1)); break;
  }

  if (act.get())
    act->perform();
}

void run_events()
{
  while (get_player()->busy_ &&
         sim_num_events() > 0)
    sim_step();
}

