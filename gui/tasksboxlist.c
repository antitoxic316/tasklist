#include "tasksboxlist.h"
#include "utils.h"

#include "path_config.h"

#include <string.h>

void tasksboxlist_show_saved_tasks(GtkWidget *tasksboxlist){
  TaskList *tasks = g_object_get_data(G_OBJECT(tasksboxlist), "tasks");
  if(!tasks){
    return;
  }
  for(; tasks != NULL; tasks = tasks->next){
    tasksboxlist_show_task(tasksboxlist, tasks->task);
  }
}

void tasksboxlist_show_task(GtkWidget *taskboxlist, Task *task){
  GtkBuilder *builder;
  GtkListBoxRow *row;
  GtkLabel *task_name_label;
  GtkTextView *task_description_textview;
  GtkTextBuffer *task_description_textbuffer;

  char *task_description = task_get_description(task);

  builder = gtk_builder_new();
  gboolean ui_load_res = gtk_builder_add_from_file(builder, LIST_BOX_ROW_UI_FILE_PATH, NULL);
  if(!ui_load_res){
    g_printerr("Error loading UI file\n");
    return;
  }

  row = GTK_LIST_BOX_ROW(gtk_builder_get_object(builder, "tasklistBoxRow"));
  
  task_name_label = GTK_LABEL(gtk_builder_get_object(builder, "taskName"));
  
  task_description_textview = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "taskDescription"));
  task_description_textbuffer = gtk_text_view_get_buffer(task_description_textview);
  gtk_text_buffer_set_text(
    task_description_textbuffer,
    task_description,
    -1
  );

  printf("%s\n", task_description);
  
  g_object_set_data(G_OBJECT(row), "task_id", task->id);
  gtk_label_set_label(task_name_label, task_get_name(task));
  
  gtk_list_box_append(GTK_LIST_BOX(taskboxlist), GTK_WIDGET(row));
  
  gtk_widget_set_size_request(GTK_WIDGET(task_description_textview), -1, 50);


}

void tasksboxlist_refresh(GtkWidget *tasksboxlist){
  gtk_list_box_remove_all(GTK_LIST_BOX(tasksboxlist));
  tasksboxlist_show_saved_tasks(tasksboxlist);
}