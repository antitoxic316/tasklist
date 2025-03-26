#ifndef TASKLIST_H
#define TASKLIST_H

#include "task.h"
#include "tasklist_serializer.h"

extern Task** tasklist_tasks;
extern size_t tasklist_tasks_count;

void tasklist_get_tasks_from_file();
void tasklist_add_task(Task *task);
void tasklist_remove_task(char *task_id);

#endif