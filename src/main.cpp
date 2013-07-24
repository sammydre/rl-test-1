#include <memory>

#include "libtcod.hpp"

struct Tile
{
  Tile()
    : ascii_('#'),
      bg_colour_(0xaa, 0xaa, 0xaa)
  {
  }

  int ascii_;
  TCODColor bg_colour_;
};

struct Map
{
  Map()
    : width_(80),
      height_(50)
  {
    tiles_.reset(new Tile[width_ * height_]);
    setup();
  }

  void setup()
  {
    for (int y = 1; y < height_ - 1; y++) {
      for (int x = 1; x < width_ - 1; x++) {
        tile_at(x, y).ascii_ = '.';
        tile_at(x, y).bg_colour_ = TCODColor(0, 0, 0);
      }
    }
  }

  void render(TCODConsole *con)
  {
    for (int y = 0; y < height_; y++) {
      for (int x = 0; x < width_; x++) {
        const Tile &t = tile_at(x, y);
        con->putChar(x, y, t.ascii_);
        con->setCharBackground(x, y, t.bg_colour_);
      }
    }
  }

  Tile &tile_at(int x, int y)
  {
    return tiles_[y * width_ + x];
  }

  int width_;
  int height_;
  std::unique_ptr<Tile[]> tiles_;
};


int main() 
{
  Map map;

  int playerx=40,playery=25;
  TCODConsole::initRoot(80,50,"libtcod C++ tutorial",false);
  while ( !TCODConsole::isWindowClosed() ) {
    TCOD_key_t key;
    // better to use waitForEvent so we don't spin?
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL);
    switch(key.vk) {
      case TCODK_UP : playery--; break;
      case TCODK_DOWN : playery++; break;
      case TCODK_LEFT : playerx--; break;
      case TCODK_RIGHT : playerx++; break;
      default:break;
    }
    TCODConsole::root->clear();
    map.render(TCODConsole::root);
    TCODConsole::root->putChar(playerx,playery,'@');
    TCODConsole::flush();
  }
  return 0;
}

