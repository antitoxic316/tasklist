#include "task.h"

#include <stdlib.h>

Task *task_init(char* task_name, time_t task_deadline){
    Task *new_task = malloc(sizeof(Task));

    new_task->creation_time = time(NULL);
    new_task->name = task_name;
    new_task->deadline = task_deadline;

    return new_task;
}