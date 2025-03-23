#include "task.h"

#include <stdlib.h>

Task *task_init(){
    Task *new_task = malloc(sizeof(Task));

    new_task->creation_time = time(NULL);

    return new_task;
}

void task_set_name(Task *task, char *task_name){
    task->name = task_name;
}


void task_set_deadline(Task *task, time_t task_deadline){
    task->deadline = task_deadline;
}

char *task_get_name(Task *task){
    return task->name;
}


time_t task_get_deadline(Task *task){
    return task->deadline;
}