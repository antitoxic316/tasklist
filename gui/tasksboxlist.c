#include "tasksboxlist.h"
#include "utils.h"

#include "tasklist.h"

#include "path_config.h"

void tasksboxlist_show_saved_tasks(GtkWidget *taskboxlist){
  tasklist_get_tasks_from_file(); //this sets tasklist_tasks and tasklist_tasks_count global variables
  for(int i = 0; i < tasklist_tasks_count; i++){
    printf("task showed %d\n", i);
    tasksboxlist_show_task(taskboxlist, tasklist_tasks[i]);
  }
}

void tasksboxlist_show_task(GtkWidget *taskboxlist, Task *task){
  GtkBuilder *builder;
  GtkListBoxRow *row;
  GtkLabel *task_name_label;

  builder = gtk_builder_new();
  gboolean ui_load_res = gtk_builder_add_from_file(builder, LIST_BOX_ROW_UI_FILE_PATH, NULL);
  if(!ui_load_res){
    g_printerr("Error loading UI file\n");
    return;
  }

  row = GTK_LIST_BOX_ROW(gtk_builder_get_object(builder, "tasklistBoxRow"));
  task_name_label = GTK_LABEL(gtk_builder_get_object(builder, "taskName"));
  
  g_object_set_data(G_OBJECT(row), "task_id", task->id);

  gtk_label_set_label(task_name_label, task->name);

  gtk_list_box_append(GTK_LIST_BOX(taskboxlist), GTK_WIDGET(row));
}