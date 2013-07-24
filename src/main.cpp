#include <memory>

#include "libtcod.hpp"

struct Tile
{
  Tile()
    : ascii_('#'),
      bg_colour_(0xaa, 0xaa, 0xaa),
      fg_colour_(0xff, 0xff, 0xff)
  {
  }

  int ascii_;
  TCODColor bg_colour_;
  TCODColor fg_colour_;
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
        tile_at(x, y).fg_colour_ = TCODColor(0x99, 0x99, 0x99);
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
        con->setCharForeground(x, y, t.fg_colour_);
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

struct Player
{
  Player()
    : x_(40),
      y_(25)
  {
  }

  void render(TCODConsole *con)
  {
    con->putChar(x_, y_, '@');
    con->setCharBackground(x_, y_, TCODColor(0, 0, 0));
    con->setCharForeground(x_, y_, TCODColor(0xff, 0xff, 0xff));
  }

  void move(int dx, int dy)
  {
    if (x_ + dx < 1 ||
        x_ + dx >= 79)
      return;
    if (y_ + dy < 1 ||
        y_ + dy >= 49)
      return;

    x_ += dx;
    y_ += dy;
  }

  int x_;
  int y_;
};


int main() 
{
  Map map;
  Player player;

  TCODConsole::initRoot(80, 50, "libtcod test", false);

  while (!TCODConsole::isWindowClosed()) {
    TCODConsole *console = TCODConsole::root;

    console->clear();
    map.render(console);
    player.render(console);

    TCODConsole::flush();

    TCOD_key_t key;
    TCOD_event_t event;

    event = TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, false);

    if (event == TCOD_EVENT_KEY_PRESS) {
      switch (key.vk) {
        case TCODK_UP:    player.move( 0, -1); break;
        case TCODK_DOWN:  player.move( 0,  1); break;
        case TCODK_LEFT:  player.move(-1,  0); break;
        case TCODK_RIGHT: player.move( 1,  0); break;
        default:
          break;
      }
    }

  }
  return 0;
}

