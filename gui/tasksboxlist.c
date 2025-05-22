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
  GObject *task_name_label;
  GObject *task_deadline_label;
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
  
  task_name_label = gtk_builder_get_object(builder, "taskName");
  task_deadline_label = gtk_builder_get_object(builder, "taskDeadline");
  
  task_description_textview = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "taskDescription"));
  task_description_textbuffer = gtk_text_view_get_buffer(task_description_textview);
  gtk_text_buffer_set_text(
    task_description_textbuffer,
    task_description,
    -1
  );

  char time_buff[32];
  time_t now = time(NULL);
  int64_t t_diff = (int64_t)difftime(task_get_deadline(task), now);
  int hours, minutes, seconds;
  hours = t_diff / 3600;
  t_diff %= 3600;
  minutes = t_diff / 60;
  t_diff %= 60;
  seconds = t_diff;
  int r = snprintf(time_buff, 32, "%d:%d:%d", hours, minutes, seconds);

  g_object_set_data(G_OBJECT(row), "task_id", task->id);

  gtk_label_set_label(GTK_LABEL(task_name_label), task_get_name(task));
  gtk_label_set_label(GTK_LABEL(task_deadline_label), time_buff);

  gtk_list_box_append(GTK_LIST_BOX(taskboxlist), GTK_WIDGET(row));
  
  gtk_widget_set_size_request(GTK_WIDGET(task_description_textview), -1, 50);
}

void tasksboxlist_refresh(GtkWidget *tasksboxlist){
  gtk_list_box_remove_all(GTK_LIST_BOX(tasksboxlist));
  tasksboxlist_show_saved_tasks(tasksboxlist);
}