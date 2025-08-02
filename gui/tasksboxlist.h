#ifndef TASKSBOXLIST_H
#define TASKSBOXLIST_H

#include <gtk/gtk.h>

#include "tasklist.h"

#include "task.h"

void tasksboxlist_show_task(GtkListBox *taskboxlist, Task *task);
void tasksboxlist_show_saved_tasks(GtkListBox *taskboxlist);
void tasksboxlist_refresh(GtkListBox *tasksboxlist);

gboolean tasksboxlist_TimeUpdateCallback(void *selfRef);

#endif