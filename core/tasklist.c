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
    if(tasks_count){
        printf("%d\n %s\n %s\n", tasks_count, tasks[0]->name, tasks[0]->description);
    }
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
    
    printf("%d", tasks_count);
    printf("tasklist len %d\n", tasklist_len(root_node));

    return root_node;
}


TaskList *tasklist_append(TaskList *tasklist, Task* task){
    tasklist_serializer_file_append(task);

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