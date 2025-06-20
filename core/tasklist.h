#ifndef TASKLIST_H
#define TASKLIST_H

#include "task.h"
#include "tasklist_serializer.h"

typedef struct _TaskList TaskList;

struct _TaskList {
    Task *task;
    TaskList *next;
    TaskList *prev;
};

int tasklist_len(TaskList *tasklist);
TaskList *tasklist_init_from_array(Task **tasks, size_t tasks_count);
TaskList *tasklist_get_tasks_from_file();
TaskList *tasklist_alloc();
TaskList *tasklist_append(TaskList *tasklist, Task* task);;
TaskList *tasklist_free_node(TaskList *tasklist, TaskList *node);
TaskList *tasklist_remove_by_id(TaskList *tasklist, char *task_id);
void tasklist_to_array(TaskList *tasklist, Task ***tasks_p, size_t *tasks_count_p);
void tasklist_free_all(TaskList *tasklist);
TaskList *tasklist_free_node_full(TaskList *tasklist, TaskList *node);

#endif