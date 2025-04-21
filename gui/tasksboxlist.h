#ifndef TASKSBOXLIST_H
#define TASKSBOXLIST_H

#include <gtk/gtk.h>

#include "tasklist.h"

#include "task.h"

void tasksboxlist_show_task(GtkWidget *taskboxlist, Task *task);
void tasksboxlist_remove_task(GtkWidget *tasksboxlist, char* id);
void tasksboxlist_show_saved_tasks(GtkWidget *taskboxlist);
void tasksboxlist_refresh(GtkWidget *tasksboxlist);

#endif