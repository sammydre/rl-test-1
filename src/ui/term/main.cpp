#include <termbox.h>

int main()
{
  tb_init();
  tb_select_output_mode(TB_OUTPUT_256);

  tb_clear();

  while (1) {
    tb_change_cell(4, 4, '@',  47, 27);

    tb_present();

    struct tb_event event;
    tb_poll_event(&event);

    break;
  }

  tb_shutdown();
  return 0;
}
