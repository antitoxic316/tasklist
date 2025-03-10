#include "mainwindow.h"
#include "tasklist_serializer.h"

#include "path_config.h"

void activate_mainwindow(GtkApplication *app, gpointer user_data){
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

  tasksBoxList = gtk_builder_get_object(builder, "taskBoxList");

  g_signal_connect_swapped(newTaskButton, "clicked", G_CALLBACK(task_init_dialog), window);
  g_signal_connect(removeTaskButton, "clicked", G_CALLBACK(remove_task_handler), NULL);

  gtk_widget_set_visible (GTK_WIDGET (window), TRUE);
  g_object_unref (builder);
}

void dialog_response_handler(GtkDialog *dialog, gint response_id){
  if(response_id == GTK_RESPONSE_ACCEPT){
    dialog_accept_handler(dialog);
  } else if(response_id == GTK_RESPONSE_REJECT) {
    dialog_reject_handler(dialog);
  }
}

void dialog_reject_handler(GtkDialog *dialog){

}

void dialog_accept_handler(GtkDialog *dialog){
    GtkRoot *root = gtk_widget_get_root (GTK_WIDGET (dialog));
    GtkWindow *window = GTK_WINDOW (root);
    GObject *taskboxlist = gtk_widget_get_template_child(GTK_WIDGET(window), GTK_TYPE_WINDOW, "taskBoxList");

    GObject *task_name_entry, *task_deadline_entry;

    Task *task;
    char *task_name;
    time_t task_deadline = 0;

    task_name_entry = gtk_widget_get_template_child(GTK_WIDGET(dialog), GTK_TYPE_DIALOG, "taskNameInput");
    task_deadline_entry = gtk_widget_get_template_child(GTK_WIDGET(dialog), GTK_TYPE_DIALOG, "taskDeadlineInput");

    GtkEntryBuffer *task_name_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(task_name_entry));
    task_name = gtk_entry_buffer_get_text(task_name_entry_buffer);

    task = task_init(task_name, task_deadline);

    taskboxlist_show_task(GTK_WIDGET(taskboxlist), task);

    free(task);
    return;
}

void taskboxlist_show_task(GtkWidget *taskboxlist, Task *task){

}

void remove_task_handler(){

}

void task_init_dialog(GtkWindow *parent){
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
}