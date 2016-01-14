#ifndef RL_MAP_HPP_
#define RL_MAP_HPP_

#include <cstdint>
#include <memory>
#include <unordered_map>

struct Colour
{
  int r_, g_, b_;
  Colour(int r, int g, int b) : r_(r), g_(g), b_(b) {}
  explicit Colour(unsigned rgb)
    : r_(rgb >> 16), g_((rgb >> 8) & 0xFF), b_(rgb & 0xFF) {}
};

struct Tile
{
  Tile();

  bool passable() const
  {
    return passable_;
  };

  char32_t char_;
  bool passable_;
  Colour bg_colour_;
  Colour fg_colour_;
};

struct TileMap
{
  void load_yaml(const char *fname);

  std::unordered_map<std::string, Tile> tiles_;
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

  bool tile_is_transparent(int x, int y) const
  {
    return true;
  }

  void set_tile_fov(int x, int y, bool fov)
  {
  }

  void clear_fov()
  {
  }

  int get_width() const { return width_; }
  int get_height() const { return height_; }

  void load_yaml(const char *fname);

  int width_;
  int height_;
  std::unique_ptr<Tile[]> tiles_;

private:
  void setup();
};

#endif // RL_MAP_HPP_
