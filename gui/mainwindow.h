#include <gtk/gtk.h>

#include "task.h"

void application_init(GtkApplication *app, gpointer user_data);
void add_task_handler();
void remove_task_handler();

void add_task_dialog_init(GtkWindow *parent);
void dialog_response_handler(GtkDialog *dialog, gint response_id);
void dialog_accept_handler(GtkDialog *dialog);
void dialog_reject_handler(GtkDialog *dialog);