#include <gtk/gtk.h>

#include "task.h"

void activate_mainwindow(GtkApplication *app, gpointer user_data);
void add_task_handler();
void remove_task_handler();

Task *task_init_dialog(GtkWindow *parent);
void taskboxlist_show_task(Task *task, GtkWidget *taskboxlist);