#ifndef RL_GUI_HPP_
#define RL_GUI_HPP_

#include <list>
#include <string>

class TCODConsole;

struct Gui
{
  Gui();

  void render(TCODConsole *);
  void add_msg(const char *msg);

private:
  static Gui* instance_;
  int messages_y_;
  int messages_x_;
  int messages_width_;
  int messages_height_;

  std::list<std::string> messages_;

  friend void gui_msg(const char*, ...);
};

void gui_msg(const char *fmt, ...);

#endif // RL_GUI_HPP_
