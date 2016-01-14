#pragma once

struct Map;
struct Player;
struct TileMap;

TileMap *get_tile_map(void);
Map *get_map(void);
Player *get_player(void);

enum ActionEnum {
  ActionMoveLeft,
  ActionMoveRight,
  ActionMoveUp,
  ActionMoveDown
};

void game_init();
void perform_action(ActionEnum e);
void run_events();
