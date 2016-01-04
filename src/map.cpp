#include "map.hpp"

Tile::Tile()
  : char_('#'),
    bg_colour_(0xaa, 0xaa, 0xaa),
    fg_colour_(0xff, 0xff, 0xff)
{
}

// ---------------------------------------------------------------------------

Map::Map()
  : width_(80),
    height_(50)
{
  tiles_.reset(new Tile[width_ * height_]);
  setup();
}

void Map::setup()
{
  for (int y = 1; y < height_ - 1; y++) {
    for (int x = 1; x < width_ - 1; x++) {
      tile_at(x, y).char_ = '.';
      tile_at(x, y).bg_colour_ = Colour(0, 0, 0);
      tile_at(x, y).fg_colour_ = Colour(0x99, 0x99, 0x99);
    }
  }
}

/*void Map::render(TCODConsole *con)
{
  for (int y = 0; y < height_; y++) {
    for (int x = 0; x < width_; x++) {
      const Tile &t = tile_at(x, y);
      con->putChar(x, y, t.ascii_);
      con->setCharBackground(x, y, t.bg_colour_);
      con->setCharForeground(x, y, t.fg_colour_);
    }
  }
}*/

