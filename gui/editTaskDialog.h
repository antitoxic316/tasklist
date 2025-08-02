#ifndef _EditTaskDialog_H_
#define _EditTaskDialog_H_

#include <gtk/gtk.h>

#include "task.h"

#define TASKAPP_TYPE_EDIT_TASK_DIALOG (taskapp_edit_task_dialog_get_type())

G_DECLARE_FINAL_TYPE (TaskappEditTaskDialog, taskapp_edit_task_dialog, TASKAPP, EDIT_TASK_DIALOG, GtkDialog);

GtkWidget *taskapp_edit_task_dialog_new(GtkWindow *parent_win, Task *t_ref);

#endif