#ifndef TASK_H
#define TASK_H

#include <time.h>

typedef struct task {
    time_t creation_time;
    char* name;
    time_t deadline;
} Task;

Task task_init(char* task_name, time_t task_deadline);

#endif