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

  GMainContext *main_context;
  GSource *update_tasks_time_src;
  guint main_context_id;

  TaskList *app_tasks;
  GtkBuilder *builder;
  GtkCssProvider *cssParser;

  main_context = g_main_context_default();
  update_tasks_time_src = g_timeout_source_new_seconds(1);

  builder = gtk_builder_new();
  gboolean ui_load_res = gtk_builder_add_from_file(builder, MAINWINDOW_UI_FILE_PATH, NULL);
  if (!ui_load_res) {
    g_printerr("Error loading UI file\n");
    return;
  }

  cssParser = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssParser, MAINWINDOW_CSS_FILE_PATH);
  gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(cssParser), GTK_STYLE_PROVIDER_PRIORITY_USER);

  window = gtk_builder_get_object(builder, "mainwindow");
  gtk_window_set_application (GTK_WINDOW (window), app);
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

  newTaskButton = gtk_builder_get_object(builder, "newTaskButton");
  removeTaskButton = gtk_builder_get_object(builder, "removeTaskButton");

  app_tasks = tasklist_get_tasks_from_file();
  tasksBoxList = gtk_builder_get_object(builder, "tasksBoxList");
  g_object_set_data(G_OBJECT(tasksBoxList), "tasks", app_tasks);

  g_signal_connect_swapped(newTaskButton, "clicked", G_CALLBACK(on_new_task_button_clicked), GTK_WINDOW(window));
  g_signal_connect_swapped(removeTaskButton, "clicked", G_CALLBACK(remove_task_handler), tasksBoxList);

  g_source_set_callback(update_tasks_time_src, tasksboxlist_TimeUpdateCallback, tasksBoxList, NULL);

  main_context_id = g_source_attach(update_tasks_time_src, main_context);

  gtk_widget_set_visible(GTK_WIDGET (window), TRUE);

  g_source_unref(update_tasks_time_src);
  g_object_unref(cssParser);
  g_object_unref(builder);

  tasksboxlist_show_saved_tasks(GTK_LIST_BOX(tasksBoxList));
}

void remove_task_handler(GtkWidget *tasksboxlist){
  TaskList *tasks = g_object_get_data(G_OBJECT(tasksboxlist), "tasks");
  GtkListBoxRow *selected_row;
  Task *task;
  char *id;

  selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(tasksboxlist)); 
  if(!selected_row){
    return;
  }

  task = g_object_get_data(G_OBJECT(selected_row), "taskRef");
  id = task_get_id(task);

  tasks = tasklist_remove_by_id(tasks, id);
  g_object_set_data(G_OBJECT(tasksboxlist), "tasks", tasks);
  tasksboxlist_refresh(GTK_LIST_BOX(tasksboxlist));
}

void on_new_task_button_clicked(GtkWindow *prnt_win){
  GtkWidget *dlg;
  dlg = taskapp_add_task_dialog_new(prnt_win);
  gtk_window_present(GTK_WINDOW(dlg));
}