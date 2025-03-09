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

  g_signal_connect_swapped(newTaskButton, "clicked", G_CALLBACK(add_task_handler), tasksBoxList);
  g_signal_connect(removeTaskButton, "clicked", G_CALLBACK(remove_task_handler), NULL);

  gtk_widget_set_visible (GTK_WIDGET (window), TRUE);
  g_object_unref (builder);
}


void add_task_handler(GtkWidget *taskboxlist){
    Task *new_task;

    GtkRoot *root = gtk_widget_get_root (GTK_WIDGET (taskboxlist));
    GtkWindow *window = GTK_WINDOW (root);

    new_task = task_init_dialog(window);
    tasklist_serializer_file_append(new_task);
    taskboxlist_show_task(new_task, taskboxlist);
}

void taskboxlist_show_task(Task *task, GtkWidget *taskboxlist){

}

void remove_task_handler(){

}

Task *task_init_dialog(GtkWindow *parent){
    Task *task;

    char *task_name = "test";
    time_t task_deadline = 5;

    GtkWidget *dialog, *label, *content_area;
    GtkDialogFlags flags;

    // Create the widgets
    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons ("Message",
                                          parent,
                                          flags,
                                          "_OK",
                                          GTK_RESPONSE_NONE,
                                          NULL);
    content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    label = gtk_label_new ("m");

    // Ensure that the dialog box is destroyed when the user responds

    g_signal_connect_swapped (dialog,
                              "response",
                              G_CALLBACK (gtk_window_destroy),
                              dialog);

    // Add the label, and show everything we’ve added

    gtk_box_append (GTK_BOX (content_area), label);
    gtk_widget_show (dialog);
    
    task = task_init(task_name, task_deadline);

    return task;
}