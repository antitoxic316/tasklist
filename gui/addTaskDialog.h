#ifndef _AddTaskDialog_H_
#define _AddTaskDialog_H_

#include <gtk/gtk.h>

#define TASKAPP_TYPE_ADD_TASK_DIALOG (taskapp_add_task_dialog_get_type())

G_DECLARE_FINAL_TYPE (TaskappAddTaskDialog, taskapp_add_task_dialog, TASKAPP, ADD_TASK_DIALOG, GtkDialog);

GtkWidget *taskapp_add_task_dialog_new(GtkWindow *parent_win);

#endif