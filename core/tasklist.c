#include "tasklist.h"

#include <stdlib.h>
#include <string.h>

TaskList *tasklist_get_tasks_from_file(){
    Task **tasks = NULL;
    size_t tasks_count = 0;
    tasklist_serializer_parse_state(&tasks, &tasks_count);
    return tasklist_init_from_array(tasks, tasks_count);
}

TaskList *tasklist_alloc(){
    TaskList *node = malloc(sizeof(TaskList));
    node->next = NULL;
    node->prev = NULL;
    return node;
}

TaskList *tasklist_init_from_array(Task **tasks, size_t tasks_count){
    if(!tasks_count){
        return NULL;
    }

    TaskList *tasklist = tasklist_alloc();
    tasklist->task = tasks[0];
    TaskList *prev_node = tasklist;
    TaskList *next_node;

    tasklist->next = next_node;
    for(int i = 1; i < tasks_count; i++){
        next_node = tasklist_alloc();
        next_node->prev = prev_node;
        next_node->task = tasks[i];
        prev_node = next_node;
        next_node = next_node->next;
    }

    return tasklist;
}


TaskList *tasklist_append(TaskList *tasklist, Task* task){
    TaskList *next_node = tasklist_alloc();

    if(!tasklist){
        next_node->task = task;
        return next_node;
    }

    TaskList *last_node = tasklist;
    while(last_node->next){ // THE BUG IS HERE !!!!!!!!!!!!!!!!!!!!!
        last_node = tasklist->next;
    }

    last_node->next = next_node;

    next_node->prev = last_node;
    next_node->task = task;
    next_node->next = NULL;

    return tasklist;
}

TaskList *tasklist_remove_node(TaskList *tasklist, TaskList *node){
    //IMPLEMENT CHECKS FOR IF node IS IN tasklist ???

    if(!node){
        return tasklist;
    }

    TaskList *node_prev = node->prev;
    TaskList *node_next = node->next;

    if(!node_prev && !node_next){
        free(node);
        return NULL;
    }

    if(!node_prev){
        free(node);   
        return node_next;
    }

    if(!node_next){
        free(node);
        return node_prev;
    }

    free(node);
    node_prev->next = node_next;
    return tasklist;
}

TaskList *tasklist_remove_by_id(TaskList *tasklist, char *task_id){
    TaskList *new_tasklist;

    for(TaskList *node = tasklist; node != NULL; node = node->next){
        Task *current_task = node->task;
        if(!strcmp(current_task->id, task_id)){
            task_free(current_task);
            new_tasklist = tasklist_remove_node(tasklist, node);

            return new_tasklist;
        }
    }

    return tasklist;
}