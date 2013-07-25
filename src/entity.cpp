#include <libtcod.hpp>

#include "entity.hpp"

Entity::Entity(int x, int y, int ascii)
  : x_(x),
    y_(y),
    ascii_(ascii)
{
}

void Entity::render(TCODConsole *con)
{
  con->putChar(x_, y_, ascii_);
  con->setCharBackground(x_, y_, TCODColor(0, 0, 0));
  con->setCharForeground(x_, y_, TCODColor(0xff, 0xff, 0xff));
}

void Entity::move(int dx, int dy)
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
