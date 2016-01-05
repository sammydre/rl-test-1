#pragma once

struct Map;
struct Player;

Map *get_map(void);
Player *get_player(void);

enum ActionEnum {
  ActionMoveLeft,
  ActionMoveRight,
  ActionMoveUp,
  ActionMoveDown
};
void perform_action(ActionEnum e);
void run_events();
