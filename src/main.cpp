#include <stdint.h>
#include <stdio.h>
#include <memory>
#include <queue>

#include "entity.hpp"
#include "event.hpp"
#include "map.hpp"
#include "gui.hpp"

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

