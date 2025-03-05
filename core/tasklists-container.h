#ifndef TASKLISTS_CONTAINER_H
#define TASKLISTS_CONTAINER_H

#include "task.h"

// CALLBACKS
void tasklist_add(Task);
void tasklist_remove(Task);
void tasklist_order_by_creation_time();

void tasklist_file_parse(char*);
void tasklist_file_save(char*);

#endif