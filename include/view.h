#include <ncurses.h>

#define L_WIN_W 50
#define C_WIN_H 3
#define C_CMD_WIDTH 30

enum log_result
{
  L_R_NONE,
  L_R_OK,
  L_R_ERR
};

enum color_palette
{
  C_BASE = 1,
  C_R_OK,
  C_R_ERR
};

struct view
{
  WINDOW *log_win,  // результат хэндлеров событий
      *status_win,  // состояние соединения
      *control_win; // подсказка управления
  void (*close)(struct view *);
  void (*log_event)(struct view *, char *, const char *, int, enum log_result);
  char cmd_buff[C_CMD_WIDTH];
  size_t log_arr_i;
};

void* view_thread(void *arg);