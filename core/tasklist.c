#include "tasklist.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

int tasklist_len(TaskList *tasklist){
    int l = 0;
    while(tasklist){
        l++;
        tasklist = tasklist->next;
    }

    return l;
}

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
    node->task = NULL;
    return node;
}

TaskList *tasklist_init_from_array(Task **tasks, size_t tasks_count){
    if(!tasks_count){
        return NULL;
    }

    TaskList *root_node = NULL;

    TaskList *tasklist = NULL;
    TaskList *prev_node = NULL;

    for(int i = 0; i < tasks_count; i++){
        tasklist = tasklist_alloc();
        tasklist->task = tasks[i];
        tasklist->prev = prev_node;
        tasklist->next = NULL;

        if(prev_node){
            prev_node->next = tasklist;
        } else {
            root_node = tasklist;
        }

        prev_node = tasklist;
    }

    return root_node;
}


TaskList *tasklist_append(TaskList *tasklist, Task* task){
    tasklist_serializer_file_append(task);

    TaskList *next_node = tasklist_alloc();
    next_node->task = task;

    if(!tasklist){
        return next_node;
    }

    TaskList *tail = tasklist;
    while(tail->next){
        tail = tail->next;
    }

    tail->next = next_node;
    next_node->prev = tail;

    return tasklist;
}

TaskList *tasklist_free_node(TaskList *tasklist, TaskList *node){
    if(!node) return tasklist;

    TaskList *new_root = tasklist;

    if(tasklist == node){
        new_root = node->next;
    }
    
    if(node->prev){
        node->prev->next = node->next;
    }
    if(node->next){
        node->next->prev = node->prev;
    }

    free(node);
    return new_root;
}

TaskList *tasklist_free_node_full(TaskList *tasklist, TaskList *node){
    if(tasklist == NULL){
        return NULL;
    }
    if(node == NULL){
        return tasklist;
    }

    task_free(node->task);
    return tasklist_free_node(tasklist, node);
}

TaskList *tasklist_remove_by_id(TaskList *tasklist, char *task_id){
    TaskList *new_tasklist;

    for(TaskList *node = tasklist; node != NULL; node = node->next){
        Task *current_task = node->task;
        if(!strcmp(current_task->id, task_id)){
            new_tasklist = tasklist_free_node_full(tasklist, node);

            Task **tasks_array_p = NULL;
            size_t tasks_count_p = 0;
            tasklist_to_array(new_tasklist, &tasks_array_p, &tasks_count_p);
            tasklist_serializer_rewrite_state(tasks_array_p, tasks_count_p);

            return new_tasklist;
        }
    }

    return tasklist;
}

void tasklist_to_array(TaskList *tasklist, Task ***tasks_p, size_t *tasks_count_p){
    Task **tasks = NULL;
    size_t tasks_count = 0;
    
    for(TaskList *node = tasklist; node != NULL; node = node->next){
        size_t new_arr_size = (tasks_count+1)*sizeof(Task*);
        tasks = realloc(tasks, new_arr_size);
        if(!tasks){
            printf("Error allocating memory for saving tasks");
            break;
        }
        tasks[tasks_count] = node->task;
        tasks_count++;
    }

    *tasks_p = tasks;
    *tasks_count_p = tasks_count;
}

void tasklist_free_all(TaskList *tasklist){
    while(tasklist){
        tasklist = tasklist_free_node_full(tasklist, tasklist);
    }
}