#ifndef TASKLIST_SERIALIZER_H
#define TASKLIST_SERIALIZER_H

#include "task.h"

void tasklist_serializer_serialize_task(Task *task);
Task tasklist_serializer_deserialize_task(char* task_name);
void tasklist_serializer_task_remove(Task* task);

void tasklist_serializer_file_append(Task* task);

void tasklist_serializer_parse_state(char* task_name);
void tasklist_serializer_save_state(char* task_name);

#endif