#ifndef RL_ENTITY_HPP_
#define RL_ENTITY_HPP_

class TCODConsole;

struct Entity
{
  Entity(int x, int y, int ascii);
  virtual void render(TCODConsole *con);
  void move(int dx, int dy);

  int x_;
  int y_;
  int ascii_;
};

#endif // RL_ENTITY_HPP_
