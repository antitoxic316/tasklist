#include "tasklist.h"

#include <stdlib.h>
#include <string.h>

Task** tasklist_tasks = NULL;
size_t tasklist_tasks_count = 0;

void tasklist_get_tasks_from_file(){
    tasklist_serializer_parse_state(&tasklist_tasks, &tasklist_tasks_count);
}

void tasklist_add_task(Task *task){
    size_t new_tasklist_tasks_size = tasklist_tasks_count*sizeof(Task*) + sizeof(Task*);
    realloc(tasklist_tasks, new_tasklist_tasks_size);

    tasklist_serializer_file_append(task);
}

void tasklist_remove_task(char *task_id){
    for(int i = 0; i < tasklist_tasks_count; i++){
        if(!strcmp(tasklist_tasks[i]->id, task_id)){
            task_free(tasklist_tasks[i]);
            tasklist_serializer_rewrite_state(tasklist_tasks, tasklist_tasks_count);

            return;
        }
    }
}