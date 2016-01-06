#include <termbox.h>

#include <climits>

#include "game.hpp"
#include "map.hpp"
#include "msg_log.hpp"
#include "pc.hpp"

int rgb_to_tbcolor(Colour rgb)
{
  const static uint32_t rgb_map[256] = {
    0x000000, 0x800000, 0x008000, 0x808000, 0x000080, 0x800080, 0x008080, 0xc0c0c0,
    0x808080, 0xff0000, 0x00ff00, 0xffff00, 0x0000ff, 0xff00ff, 0x00ffff, 0xffffff,
    0x000000, 0x00005f, 0x000087, 0x0000af, 0x0000d7, 0x0000ff, 0x005f00, 0x005f5f,
    0x005f87, 0x005faf, 0x005fd7, 0x005fff, 0x008700, 0x00875f, 0x008787, 0x0087af,
    0x0087d7, 0x0087ff, 0x00af00, 0x00af5f, 0x00af87, 0x00afaf, 0x00afd7, 0x00afff,
    0x00d700, 0x00d75f, 0x00d787, 0x00d7af, 0x00d7d7, 0x00d7ff, 0x00ff00, 0x00ff5f,
    0x00ff87, 0x00ffaf, 0x00ffd7, 0x00ffff, 0x5f0000, 0x5f005f, 0x5f0087, 0x5f00af,
    0x5f00d7, 0x5f00ff, 0x5f5f00, 0x5f5f5f, 0x5f5f87, 0x5f5faf, 0x5f5fd7, 0x5f5fff,
    0x5f8700, 0x5f875f, 0x5f8787, 0x5f87af, 0x5f87d7, 0x5f87ff, 0x5faf00, 0x5faf5f,
    0x5faf87, 0x5fafaf, 0x5fafd7, 0x5fafff, 0x5fd700, 0x5fd75f, 0x5fd787, 0x5fd7af,
    0x5fd7d7, 0x5fd7ff, 0x5fff00, 0x5fff5f, 0x5fff87, 0x5fffaf, 0x5fffd7, 0x5fffff,
    0x870000, 0x87005f, 0x870087, 0x8700af, 0x8700d7, 0x8700ff, 0x875f00, 0x875f5f,
    0x875f87, 0x875faf, 0x875fd7, 0x875fff, 0x878700, 0x87875f, 0x878787, 0x8787af,
    0x8787d7, 0x8787ff, 0x87af00, 0x87af5f, 0x87af87, 0x87afaf, 0x87afd7, 0x87afff,
    0x87d700, 0x87d75f, 0x87d787, 0x87d7af, 0x87d7d7, 0x87d7ff, 0x87ff00, 0x87ff5f,
    0x87ff87, 0x87ffaf, 0x87ffd7, 0x87ffff, 0xaf0000, 0xaf005f, 0xaf0087, 0xaf00af,
    0xaf00d7, 0xaf00ff, 0xaf5f00, 0xaf5f5f, 0xaf5f87, 0xaf5faf, 0xaf5fd7, 0xaf5fff,
    0xaf8700, 0xaf875f, 0xaf8787, 0xaf87af, 0xaf87d7, 0xaf87ff, 0xafaf00, 0xafaf5f,
    0xafaf87, 0xafafaf, 0xafafd7, 0xafafff, 0xafd700, 0xafd75f, 0xafd787, 0xafd7af,
    0xafd7d7, 0xafd7ff, 0xafff00, 0xafff5f, 0xafff87, 0xafffaf, 0xafffd7, 0xafffff,
    0xd70000, 0xd7005f, 0xd70087, 0xd700af, 0xd700d7, 0xd700ff, 0xd75f00, 0xd75f5f,
    0xd75f87, 0xd75faf, 0xd75fd7, 0xd75fff, 0xd78700, 0xd7875f, 0xd78787, 0xd787af,
    0xd787d7, 0xd787ff, 0xd7af00, 0xd7af5f, 0xd7af87, 0xd7afaf, 0xd7afd7, 0xd7afff,
    0xd7d700, 0xd7d75f, 0xd7d787, 0xd7d7af, 0xd7d7d7, 0xd7d7ff, 0xd7ff00, 0xd7ff5f,
    0xd7ff87, 0xd7ffaf, 0xd7ffd7, 0xd7ffff, 0xff0000, 0xff005f, 0xff0087, 0xff00af,
    0xff00d7, 0xff00ff, 0xff5f00, 0xff5f5f, 0xff5f87, 0xff5faf, 0xff5fd7, 0xff5fff,
    0xff8700, 0xff875f, 0xff8787, 0xff87af, 0xff87d7, 0xff87ff, 0xffaf00, 0xffaf5f,
    0xffaf87, 0xffafaf, 0xffafd7, 0xffafff, 0xffd700, 0xffd75f, 0xffd787, 0xffd7af,
    0xffd7d7, 0xffd7ff, 0xffff00, 0xffff5f, 0xffff87, 0xffffaf, 0xffffd7, 0xffffff,
    0x080808, 0x121212, 0x1c1c1c, 0x262626, 0x303030, 0x3a3a3a, 0x444444, 0x4e4e4e,
    0x585858, 0x626262, 0x6c6c6c, 0x767676, 0x808080, 0x8a8a8a, 0x949494, 0x9e9e9e,
    0xa8a8a8, 0xb2b2b2, 0xbcbcbc, 0xc6c6c6, 0xd0d0d0, 0xdadada, 0xe4e4e4, 0xeeeeee
  };

  int best_cost = -1, best_index = 0;

  for (int i = 0; i < 256; i++) {
    int b = (rgb_map[i] >> 0) & 0xFF,
        g = (rgb_map[i] >> 8) & 0xFF,
        r = (rgb_map[i] >> 16) & 0xFF;
    int d1 = rgb.r_ - r,
        d2 = rgb.g_ - g,
        d3 = rgb.b_ - b;
    int cost = d1 * d1 + d2 * d2 + d3 * d3;

    if (best_cost < 0 || cost < best_cost) {
      best_cost = cost;
      best_index = i;
    }
  }

  return best_index;
}

struct Framebuffer
{
  Framebuffer(int cx, int cy)
    : center_x_(cx), center_y_(cy),
      tb_width_(tb_width()), tb_height_(tb_height()),
      tb_cells_(tb_cell_buffer())
  {}

  void render_map(Map *m)
  {
    const int map_size_x = tb_width_;
    const int map_size_y = tb_height_;

    for (int y = 0; y < map_size_y; y++) {
      for (int x = 0; x < map_size_x; x++) {
        struct tb_cell *c = &tb_cells_[y * tb_width_ + x];

        const int mx = tb_coord_to_map_x(x);
        const int my = tb_coord_to_map_y(y);

        if (m->contains(mx, my)) {
          Tile &tile = m->tile_at(mx, my);
          c->ch = tile.char_;
          c->fg = rgb_to_tbcolor(tile.fg_colour_);
          c->bg = rgb_to_tbcolor(tile.bg_colour_);
        } else {
          c->ch = ' ';
          c->fg = 0;
          c->bg = 0;
        }
      }
    }
  }

  void render_entity(Entity *e)
  {
    int tx = map_coord_to_tb_x(e->x_);
    int ty = map_coord_to_tb_y(e->y_);

    if (tb_coord_valid(tx, ty)) {
      struct tb_cell *cell = &tb_cells_[ty * tb_width_ + tx];
      cell->ch = e->char_;
      cell->fg = rgb_to_tbcolor(Colour(255, 0, 0));
      cell->bg = 0;
    }
  }

  void render_msg_log(const MessageList &ml)
  {
    if (ml.empty())
      return;

    std::string s = ml.back().second;
    render_string(s, 1, tb_height_ - 1);
  }

  void render_string(const std::string &str, int x, int y)
  {
    for (std::string::size_type i = 0; i < str.size(); i++) {
      if (tb_coord_valid(x + i, y)) {
        struct tb_cell *cell = &tb_cells_[y * tb_width_ + x + i];
        cell->ch = str[i];
        cell->fg = rgb_to_tbcolor(Colour(120, 120, 120));
        cell->bg = 0;
      }
    }
  }

  int get_height() const { return tb_height_; }
  int get_width() const { return tb_width_; }

private:
  int map_coord_to_tb_x(int x)
  {
    return x - center_x_ + tb_width_/2;
  }
  int map_coord_to_tb_y(int y)
  {
    return y - center_y_ + tb_height_/2;
  }
  int tb_coord_to_map_x(int x)
  {
    return x + center_x_ - tb_width_/2;
  }
  int tb_coord_to_map_y(int y)
  {
    return y + center_y_ - tb_height_/2;
  }
  bool tb_coord_valid(int x, int y)
  {
    return x >= 0 && x < tb_width_ &&
           y >= 0 && y < tb_height_;
  }

  int center_x_;
  int center_y_;
  int tb_width_;
  int tb_height_;
  struct tb_cell *tb_cells_;
};

struct View
{
  virtual void render(Framebuffer *fb) = 0;
  virtual void handle_key(struct tb_event *ev) = 0;
};

void navigate(View *);

struct DebugView : public View
{
  void render(Framebuffer *fb) override;
  void handle_key(struct tb_event *ev) override;
};

struct NormalView : public View
{
  void render(Framebuffer *fb) override;
  void handle_key(struct tb_event *ev) override;
};

void NormalView::render(Framebuffer *fb)
{
  auto m = get_map();
  auto p = get_player();
  auto &msg_log = get_msg_log();

  fb->render_map(m);
  fb->render_entity(p);
  fb->render_msg_log(msg_log);
}

void NormalView::handle_key(struct tb_event *ev)
{
  switch (ev->key) {
    case TB_KEY_CTRL_C:
      break;
    case TB_KEY_ARROW_LEFT:
      perform_action(ActionMoveLeft);
      break;
    case TB_KEY_ARROW_RIGHT:
      perform_action(ActionMoveRight);
      break;
    case TB_KEY_ARROW_UP:
      perform_action(ActionMoveUp);
      break;
    case TB_KEY_ARROW_DOWN:
      perform_action(ActionMoveDown);
      break;
  }
  switch (ev->ch) {
    case '`':
      navigate(new DebugView());
      break;
  }
}

void DebugView::render(Framebuffer *fb)
{
  tb_clear();

  auto &msg_log = get_dbg_log();
  int y = fb->get_height() - 1;

  for (auto i = msg_log.rbegin(); i != msg_log.rend() && y >= 0; ++i, --y) {
    fb->render_string(i->second, 0, y);
  }
}

void DebugView::handle_key(struct tb_event *ev)
{
  switch (ev->ch) {
    case '`':
      navigate(new NormalView());
      break;
  }
}

namespace {
  std::unique_ptr<View> view(new NormalView());
};

void navigate(View *v)
{
  view.reset(v);
}

int main()
{
  int err = tb_init();
  if (err < 0) {
    fprintf(stderr, "tb_init: %d\n", err);
    return 1;
  }

  atexit(&tb_shutdown);

  tb_select_output_mode(TB_OUTPUT_256);

  tb_clear();

  while (1) {
    auto p = get_player();
    Framebuffer fb(p->x_, p->y_);
    view->render(&fb);
    tb_present();

    struct tb_event event;
    tb_poll_event(&event);

    if (event.type == TB_EVENT_KEY) {
      if (event.key == TB_KEY_CTRL_C)
        exit(0);

      view->handle_key(&event);
    }
    else if (event.type == TB_EVENT_RESIZE)
      continue;
    else if (event.type == TB_EVENT_MOUSE)
      ;

    run_events();
  }

  return 0;
}
