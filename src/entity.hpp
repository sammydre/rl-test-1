#ifndef RL_ENTITY_HPP_
#define RL_ENTITY_HPP_

#include "debug.hpp"

struct Entity : public DebugDescription
{
  Entity(int x, int y, int char32);

  void move(int dx, int dy);

  void set_busy(bool is_busy)
  {
    busy_ = is_busy;
  }

  bool is_busy(void) const { return busy_; }

  int x_;
  int y_;
  char32_t char_;
  bool busy_;
};

#endif // RL_ENTITY_HPP_
