#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <gtk/gtk.h>

#include "task.h"

void application_init(GtkApplication *app, gpointer user_data);
void add_task_handler();
void remove_task_handler(GtkWidget *taskboxlist);
void on_new_task_button_clicked(GtkWindow *prnt_win);

#endif