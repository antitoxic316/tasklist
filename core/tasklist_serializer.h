#ifndef TASKLIST_SERIALIZER_H
#define TASKLIST_SERIALIZER_H

#include "task.h"

void tasklist_serializer_file_append(Task* task);

void tasklist_serializer_parse_state(Task ***tasks, size_t *tasks_count);
void tasklist_serializer_rewrite_state(Task **tasks, size_t tasks_count);

#endif