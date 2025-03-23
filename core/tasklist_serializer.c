#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tasklist_serializer.h"

#include "core_path_config.h"

void tasklist_serializer_file_append(Task *task){
    FILE *file = fopen(APPDATA_FILE, "ab");

    if(file == NULL){
        printf("error opening file\n");
        return;
    }
    size_t res;

    size_t task_name_len = strlen(task->name);

    res = fwrite(&(task->creation_time), sizeof(time_t), 1, file);
    res = fwrite(&task_name_len, sizeof(size_t), 1, file);
    res = fwrite(task->name, sizeof(char), task_name_len, file);
    res = fwrite(&(task->deadline), sizeof(time_t), 1, file);

    fclose(file);
}

void tasklist_serializer_parse_state(Task*** tasks, size_t *tasks_count){
    Task **task_array = NULL;
    FILE *file = fopen(APPDATA_FILE, "rb");

    if(file == NULL){
        printf("error opening file\n");
        return;
    }

    size_t count = 0;

    while(1){
        Task *current_task = malloc(1 * sizeof(Task));
        size_t task_name_len;
        char *task_name;
        
        
        fread(&(current_task->creation_time), sizeof(time_t), 1, file);
        if(feof(file)){
            free(current_task);
            break;
        }

        fread(&task_name_len, sizeof(size_t), 1, file);
        if(feof(file)){
            free(current_task);
            break;
        }
        
        task_name = malloc(task_name_len+1);
        fread(task_name, sizeof(char), task_name_len, file);
        task_name[task_name_len] = '\0';
        if(feof(file)){
            free(task_name);
            free(current_task);
            break;
        }
        
        fread(&(current_task->deadline), sizeof(time_t), 1, file);
        if(feof(file)){
            free(task_name);
            free(current_task);
            break;
        }

        current_task->name = task_name;

        count++;
        task_array = realloc(task_array, count*sizeof(Task*));
        task_array[count-1] = current_task;

        printf("%s\n", task_array[0]->name);
    }

    fclose(file);
    *tasks = task_array;
    *tasks_count = count;
}