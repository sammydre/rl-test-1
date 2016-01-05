#ifndef RL_MAP_HPP_
#define RL_MAP_HPP_

#include <cstdint>
#include <memory>

struct Colour
{
  int r_, g_, b_;
  Colour(int r, int g, int b) : r_(r), g_(g), b_(b) {}
};

struct Tile
{
  Tile();

  bool passable() const
  {
    return char_ == '.';
  };

  char32_t char_;
  Colour bg_colour_;
  Colour fg_colour_;
};

struct Map
{
  Map();

  bool contains(int x, int y) const
  {
    return x >= 0 && x < width_ &&
           y >= 0 && y < height_;
  }

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
