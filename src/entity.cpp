#include "entity.hpp"

Entity::Entity(int x, int y, int ascii)
  : x_(x),
    y_(y),
    ascii_(ascii)
{
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
