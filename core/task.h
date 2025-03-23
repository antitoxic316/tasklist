#ifndef TASK_H
#define TASK_H

#include <time.h>

typedef struct task {
    time_t creation_time;
    char* name;
    time_t deadline;
} Task;

Task *task_init();
void task_set_name(Task *task, char *task_name);
void task_set_deadline(Task *task, time_t task_deadline);

char *task_get_name(Task *task);
time_t task_get_deadline(Task *task);



#endif