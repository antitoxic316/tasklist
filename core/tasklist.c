#include "tasklist.h"

Task** tasklist_tasks = NULL;
size_t tasklist_tasks_count = 0;

void tasklist_get_tasks_from_file(){
    tasklist_serializer_parse_state(&tasklist_tasks, &tasklist_tasks_count);
}