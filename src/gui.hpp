#ifndef RL_GUI_HPP_
#define RL_GUI_HPP_

#include <list>
#include <string>

class TCODConsole;

struct Gui
{
  Gui();

  // void render(TCODConsole *);
  void add_msg(const char *msg);
  bool is_dirty() const;
  void set_dirty(bool is_dirty);

private:
  static Gui* instance_;
  int messages_y_;
  int messages_x_;
  int messages_width_;
  int messages_height_;
  bool dirty_;

  std::list<std::string> messages_;

  friend void gui_msg(const char*, ...);
  friend void gui_set_dirty(void);
  friend void gui_set_clean(void);
};

void gui_msg(const char *fmt, ...);
void gui_set_dirty(void);
void gui_set_clean(void);

#endif // RL_GUI_HPP_
