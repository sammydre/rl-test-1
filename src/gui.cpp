#include <iterator>
#include <stdarg.h>
#include <stdio.h>

#include "gui.hpp"

// #include <libtcod.hpp>

Gui *Gui::instance_ = NULL;

Gui::Gui()
  : messages_y_(51),
    messages_x_(0),
    messages_width_(78),
    messages_height_(18),
    dirty_(true)
{
  Gui::instance_ = this;
}

/*void Gui::render(TCODConsole *con)
{
  int messages_to_draw = std::min(
      (size_t)messages_height_,
      messages_.size());

  if (messages_to_draw == 0)
    return;

  int x = messages_x_;
  int y = messages_y_;
  auto i = std::begin(messages_);
  std::advance(i, messages_.size() - messages_to_draw);

  for (; i != std::end(messages_); ++i, ++y) {
    con->print(x, y, "%s", (*i).c_str());
  }
}*/

void Gui::add_msg(const char *msg)
{
  messages_.push_back(std::string(msg));
  if (messages_.size() > 200)
    messages_.pop_front();
}

bool Gui::is_dirty() const
{
  return dirty_;
}

void Gui::set_dirty(bool is_dirty)
{
  dirty_ = is_dirty;
}


void gui_msg(const char *fmt, ...)
{
  char buffer[256];
  va_list args;
  va_start(args, fmt);
  vsprintf(buffer, fmt, args);
  va_end(args);

  Gui::instance_->add_msg(buffer);
}

void gui_set_dirty(void)
{
  Gui::instance_->set_dirty(true);
}

void gui_set_clean(void)
{
  Gui::instance_->set_dirty(false);
}

