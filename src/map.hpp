#ifndef RL_MAP_HPP_
#define RL_MAP_HPP_

#include <memory>

#include <libtcod.hpp>

struct Tile
{
  Tile();

  int ascii_;
  TCODColor bg_colour_;
  TCODColor fg_colour_;
};

struct Map
{
  Map();

  void render(TCODConsole *con);

  Tile &tile_at(int x, int y)
  {
    return tiles_[y * width_ + x];
  }

  int width_;
  int height_;
  std::unique_ptr<Tile[]> tiles_;

private:
  void setup();
};

#endif // RL_MAP_HPP_
