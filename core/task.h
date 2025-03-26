#ifndef TASK_H
#define TASK_H

#include <time.h>

#define TASK_ID_LEN 16

typedef struct task {
    char *id;
    time_t creation_time;
    char *name;
    char *description;
    time_t deadline;
} Task;

Task *task_init();
void task_free(Task *task); //
void task_set_name(Task *task, char *task_name);
void task_set_description(Task *task, char *task_description); //
void task_set_deadline(Task *task, time_t task_deadline); 

char *task_get_id(Task *task);
char *task_get_name(Task *task);
char *task_get_description(Task *task); //
time_t task_get_deadline(Task *task); 
time_t task_get_creation_time(Task *task); //



#endif