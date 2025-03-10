#include <gtk/gtk.h>

#include "task.h"

void activate_mainwindow(GtkApplication *app, gpointer user_data);
void add_task_handler();
void remove_task_handler();

void taskboxlist_show_task(GtkWidget *taskboxlist, Task *task);

void task_init_dialog(GtkWindow *parent);
void dialog_response_handler(GtkDialog *dialog, gint response_id);
void dialog_accept_handler(GtkDialog *dialog);
void dialog_reject_handler(GtkDialog *dialog);