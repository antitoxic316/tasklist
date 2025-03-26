#include "task.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

Task *task_init(){
    Task *new_task = malloc(sizeof(Task));

    new_task->id = NULL;
    new_task->name = NULL;

    new_task->creation_time = time(NULL);
    uint seed = time(0);

    char *task_id = malloc(1*TASK_ID_LEN);
    int rd_num = rand_r(&seed) % (int)(pow(10, TASK_ID_LEN+1) - pow(10, TASK_ID_LEN) + 1) + 10*TASK_ID_LEN;
    printf("%d\n", rd_num);
    int count = TASK_ID_LEN-1;
    while(--count){
        task_id[count] = rd_num%10 + '0';
        printf("%d\n", count);
        printf("%c\n", task_id[count]);
        rd_num /= 10;
    }

    task_id[TASK_ID_LEN-1] = '\0';

    new_task->id = task_id;

    printf("f");

    return new_task;
}

void task_free(Task *task){
    return;
    printf("freeing task");
    if(!task){
        return;
    }

    if(task->name){
        free(task->name);
    }
    if(task->id){
        free(task->id);
    }

    free(task);
    task = NULL;
}

void task_set_name(Task *task, char *task_name){
    task->name = task_name;
}

void task_set_description(Task *task, char *task_description){
    task->description = task_description;
}

void task_set_deadline(Task *task, time_t task_deadline){
    task->deadline = task_deadline;
}

char *task_get_name(Task *task){
    return task->name;
}

char *task_get_description(Task *task){
    return task->description;
}

time_t task_get_deadline(Task *task){
    return task->deadline;
}

char *task_get_id(Task* task){
    return task->id;
}