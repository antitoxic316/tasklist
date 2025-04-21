#include "mainwindow.h"
#include "tasklist.h"

#include "utils.h"

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

void dialog_response_handler(GtkDialog *dialog, gint response_id){
  if(response_id == GTK_RESPONSE_ACCEPT){
    dialog_accept_handler(dialog);
  } else if(response_id == GTK_RESPONSE_REJECT) {
    dialog_reject_handler(dialog);
  }
}

void dialog_reject_handler(GtkDialog *dialog){
    return;
}

void dialog_accept_handler(GtkDialog *dialog){
    GtkWindow *mainwindow;
    GtkWidget *tasksboxlist, *task_name_entry, *task_deadline_entry;
    GtkCalendar *calendar_entry;
    GtkWidget *task_description_entry;
    GtkWidget *task_time_entry;
    GtkEntryBuffer *task_name_entry_buffer;
    GtkEntryBuffer *task_description_entry_buffer;

    mainwindow = gtk_window_get_transient_for(GTK_WINDOW(dialog));

    tasksboxlist = gtk_widget_get_child_by_name(gtk_window_get_child(mainwindow), "tasksBoxList");
    task_name_entry = gtk_widget_get_child_by_name(GTK_WIDGET(dialog), "taskNameInput");
    task_name_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(task_name_entry));
    task_description_entry = gtk_widget_get_child_by_name(GTK_WIDGET(dialog), "taskDescriptionInput");
    task_description_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(task_description_entry));

    TaskList *app_tasks;
    app_tasks = g_object_get_data(G_OBJECT(tasksboxlist), "tasks");

    Task *task;
    char *task_name;
    char *task_description;
    time_t task_deadline = 50;

    task_name = g_strdup(gtk_entry_buffer_get_text(task_name_entry_buffer));
    task_description = g_strdup(gtk_entry_buffer_get_text(task_description_entry_buffer));
    
    task = task_init();
    task_set_name(task, task_name);
    task_set_description(task, task_description);
    task_set_deadline(task, 5);

    app_tasks = tasklist_append(app_tasks, task);
    g_object_set_data(G_OBJECT(tasksboxlist), "tasks", app_tasks);
    tasksboxlist_refresh(GTK_WIDGET(tasksboxlist));
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

void add_task_dialog_init(GtkWindow *parent){
  GtkBuilder *builder;
  GtkWidget *dialog, *content_area;
  GObject *content_box;
  GtkDialogFlags flags;

  builder = gtk_builder_new();
  gboolean ui_load_res = gtk_builder_add_from_file(builder, TASK_INIT_DIALOG_UI_FILE_PATH, NULL);
  if (!ui_load_res) {
    g_printerr("Error loading UI file\n");
    return;
  }

  flags = GTK_DIALOG_DESTROY_WITH_PARENT;
  dialog = gtk_dialog_new_with_buttons("Task creation",
                                        parent,
                                        flags,
                                        "_OK",
                                        GTK_RESPONSE_ACCEPT,
                                        "_Cancel",
                                        GTK_RESPONSE_REJECT,
                                        NULL);
  content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  content_box = gtk_builder_get_object(builder, "contentArea");

  g_signal_connect(dialog, "response", G_CALLBACK(dialog_response_handler), NULL);
  g_signal_connect_swapped(dialog,
                            "response",
                            G_CALLBACK (gtk_window_destroy),
                            dialog);
  gtk_box_append(GTK_BOX(content_area), GTK_WIDGET(content_box));
  gtk_widget_show(dialog);

  g_object_unref(builder);
}