#include "mainwindow.h"

#include "utils.h"
#include "tasklist.h"

#include "addTaskDialog.h"
#include "tasksboxlist.h"

#include "path_config.h"

void application_init(GtkApplication *app, gpointer user_data){
  GObject *window;
  GObject *newTaskButton;
  GObject *removeTaskButton;
  GObject *tasksBoxList;
  
  GtkBuilder *builder = gtk_builder_new();
  gboolean ui_load_res = gtk_builder_add_from_file(builder, MAINWINDOW_UI_FILE_PATH, NULL);
  if (!ui_load_res) {
    g_printerr("Error loading UI file\n");
    return;
  }

  GtkCssProvider *cssParser = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssParser, MAINWINDOW_CSS_FILE_PATH);
  gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(cssParser), GTK_STYLE_PROVIDER_PRIORITY_USER);

  window = gtk_builder_get_object(builder, "mainwindow");
  gtk_window_set_application (GTK_WINDOW (window), app);

  newTaskButton = gtk_builder_get_object(builder, "newTaskButton");
  removeTaskButton = gtk_builder_get_object(builder, "removeTaskButton");

  TaskList *app_tasks;
  app_tasks = tasklist_get_tasks_from_file();
  tasksBoxList = gtk_builder_get_object(builder, "tasksBoxList");
  g_object_set_data(G_OBJECT(tasksBoxList), "tasks", app_tasks);

  g_signal_connect_swapped(newTaskButton, "clicked", G_CALLBACK(add_task_dialog_init), window);
  g_signal_connect_swapped(removeTaskButton, "clicked", G_CALLBACK(remove_task_handler), tasksBoxList);

  gtk_widget_set_visible(GTK_WIDGET (window), TRUE);
  g_object_unref(builder);

  tasksboxlist_show_saved_tasks(GTK_WIDGET(tasksBoxList));
}

void remove_task_handler(GtkWidget *tasksboxlist){
  TaskList *tasks = g_object_get_data(G_OBJECT(tasksboxlist), "tasks");
  GtkListBoxRow *selected_row;
  char *id;

  selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(tasksboxlist)); 
  if(!selected_row){
    return;
  }

  id = g_object_get_data(G_OBJECT(selected_row), "task_id");

  tasks = tasklist_remove_by_id(tasks, id);
  g_object_set_data(G_OBJECT(tasksboxlist), "tasks", tasks);
  tasksboxlist_refresh(GTK_WIDGET(tasksboxlist));
}