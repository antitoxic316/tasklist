#ifndef _AddTaskDialog_H_
#define _AddTaskDialog_H_

#include <gtk/gtk.h>

#include "tasklist.h"

void add_task_dialog_init(GtkWindow *parent);
void add_task_dialog_response_handler(GtkDialog *dialog, gint response_id);
void add_task_dialog_accept(GtkDialog *dialog);
void add_task_dialog_reject(GtkDialog *dialog);
Task *add_task_dialog_parse_task(GtkDialog *dialog);

#endif