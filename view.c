#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <pthread.h>

#include "include/view.h"

void view_log_event(struct view *view, char text[], const char fun[], int line, enum log_result result)
{
  char to_print[L_WIN_W - 2 - 5]; // Необходима для расчета ширины текста и добавления скобочек результата после него

  int y_off = 1 + +view->log_arr_i;

  sprintf(to_print, "%s %s:%i", text, fun, line);
  mvwprintw(view->log_win, y_off, 1, "%s", to_print);

  switch (result)
  {
  case L_R_OK:
    wattron(view->log_win, COLOR_PAIR(C_BASE));
    mvwprintw(view->log_win, y_off, 1 + strlen(to_print) + 1, "%s", "[  ]");
    wattron(view->log_win, COLOR_PAIR(C_R_OK));
    mvwprintw(view->log_win, y_off, 1 + strlen(to_print) + 2, "%s", "OK");
    wattron(view->log_win, COLOR_PAIR(C_BASE));
    break;
  case L_R_ERR:
    wattron(view->log_win, COLOR_PAIR(C_BASE));
    mvwprintw(view->log_win, y_off, 1 + strlen(to_print) + 1, "%s", "[   ]");
    wattron(view->log_win, COLOR_PAIR(C_R_ERR));
    mvwprintw(view->log_win, y_off, 1 + strlen(to_print) + 2, "%s", "ERR");
    wattron(view->log_win, COLOR_PAIR(C_BASE));
    break;
  }

  if (++view->log_arr_i >= (LINES - C_WIN_H - 2)) //Индекс следущей записи
  {
    view->log_arr_i = 0;
    werase(view->log_win);
  }
}

void view_close(struct view *view)
{
  delwin(view->log_win);
  delwin(view->status_win);
  delwin(view->control_win);
  endwin();
  view->log_event(view, "View was cleaned", __func__, __LINE__, L_R_OK);
  free(view);

}

void *cmd_thread(void *arg)
{
  struct view *view = arg;

  view->log_event(view, "View cmd start thread", __func__, __LINE__, L_R_OK);


  int cmd_buff_i = 0;
  char ch;
  while (1)
  {
    ch = (char)getchar();
    if (ch == '\r')
    {
      cmd_buff_i = 0;
      memset(view->cmd_buff, 0, (C_CMD_WIDTH + 1));
      werase(view->control_win);
    }
    else
    {
      view->cmd_buff[cmd_buff_i++] = ch;
      if (cmd_buff_i > C_CMD_WIDTH)
      {
        cmd_buff_i = 0;
        memset(view->cmd_buff, 0, (C_CMD_WIDTH + 1));
        werase(view->control_win);
      }
    }
  }
}

void *view_thread(void *arg)
{
  initscr();
  // keypad(stdscr, TRUE);
  curs_set(0);
  start_color();
  refresh(); /* Print it on to the real screen */

  struct view *view = arg;

  view->log_win = newwin(LINES - C_WIN_H, L_WIN_W, 0, 0);
  view->status_win = newwin(LINES - C_WIN_H, COLS - L_WIN_W, 0, L_WIN_W);
  view->control_win = newwin(C_WIN_H, COLS, LINES - C_WIN_H, 0);
  view->close = &view_close;
  view->log_event = &view_log_event;
  view->log_arr_i = 0;

  init_color(COLOR_BLACK,0,0,0);
  init_color(COLOR_GREEN,0,1000,0);
  init_color(COLOR_RED,1000,0,0);

  init_pair(C_BASE, COLOR_WHITE, COLOR_BLACK);
  init_pair(C_R_OK, COLOR_GREEN, COLOR_BLACK);
  init_pair(C_R_ERR, COLOR_RED, COLOR_BLACK);

  int cmd_buff_i = 0;
  pthread_t cmd_thread_id;
  pthread_create(&cmd_thread_id, NULL, cmd_thread, view);

  while (1)
  {

    box(view->log_win, 0, 0);
    box(view->status_win, 0, 0);
    box(view->control_win, 0, 0);

    wattron(view->log_win, COLOR_PAIR(C_BASE));
    mvwprintw(view->control_win, 1, 1, "%s", view->cmd_buff);

    wrefresh(view->log_win);
    wrefresh(view->status_win);
    wrefresh(view->control_win);
  }
}