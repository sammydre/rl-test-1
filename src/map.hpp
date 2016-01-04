#ifndef RL_MAP_HPP_
#define RL_MAP_HPP_

#include <cstdint>
#include <memory>

struct Colour
{
  uint8_t r_, g_, b_;
  Colour(int r, int g, int b) : r_(r), g_(g), b_(b) {}
};

struct Tile
{
  Tile();

  char32_t char_;
  Colour bg_colour_;
  Colour fg_colour_;
};

struct Map
{
  Map();

  // void render(TCODConsole *con);

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
