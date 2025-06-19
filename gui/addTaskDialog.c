#include "addTaskDialog.h"

#include "utils.h"
#include "timeSpinButton.h"

#include "tasksboxlist.h"

#include "path_config.h"

void add_task_dialog_init(GtkWindow *parent){
  GtkBuilder *builder;
  GtkBuilderScope *scope;
  GtkWidget *dialog, *content_area;
  GObject *content_box;
  GtkDialogFlags flags;
  GObject *time_spinbutton;
  GtkAdjustment *adjustment;

  builder = gtk_builder_new();
  scope = gtk_builder_cscope_new();
  gtk_builder_cscope_add_callback(GTK_BUILDER_CSCOPE (scope), time_spinbutton_input);
  gtk_builder_cscope_add_callback(GTK_BUILDER_CSCOPE (scope), time_spinbutton_output);
  
  gtk_builder_set_scope(builder, scope);

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

  time_spinbutton = gtk_builder_get_object(builder, "timeInput");
  adjustment = gtk_adjustment_new(1439.0, 0.0, 1439.0, 15.0, 60.0, 0.0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(time_spinbutton), adjustment);

  g_signal_connect(dialog, "response", G_CALLBACK(add_task_dialog_response_handler), NULL);
  g_signal_connect_swapped(dialog,
                            "response",
                            G_CALLBACK (gtk_window_destroy),
                            dialog);
  gtk_box_append(GTK_BOX(content_area), GTK_WIDGET(content_box));
  gtk_widget_show(dialog);

  g_object_unref(builder);
}

void add_task_dialog_response_handler(GtkDialog *dialog, gint response_id){
  if(response_id == GTK_RESPONSE_ACCEPT){
    add_task_dialog_accept(dialog);
  } else if(response_id == GTK_RESPONSE_REJECT) {
    add_task_dialog_reject(dialog);
  }
}

void add_task_dialog_reject(GtkDialog *dialog){
    return;
}

void add_task_dialog_accept(GtkDialog *dialog){
    GtkWindow *mainwindow;
    GtkWidget *tasksboxlist;

    mainwindow = gtk_window_get_transient_for(GTK_WINDOW(dialog));

    tasksboxlist = gtk_widget_get_child_by_name(gtk_window_get_child(mainwindow), "tasksBoxList");

    TaskList *app_tasks;
    app_tasks = g_object_get_data(G_OBJECT(tasksboxlist), "tasks");

    Task *new_task = add_task_dialog_parse_task(dialog);

    app_tasks = tasklist_append(app_tasks, new_task);
    g_object_set_data(G_OBJECT(tasksboxlist), "tasks", app_tasks);
    tasksboxlist_refresh(GTK_LIST_BOX(tasksboxlist));
}

Task *add_task_dialog_parse_task(GtkDialog *dialog){
  Task *task;
  char *task_name;
  char *task_description;
  time_t task_deadline;

  GtkWidget *task_name_entry, *task_description_entry;
  GtkWidget *task_minutes_entry;
  GtkWidget *task_hours_entry; 
  GtkWidget *task_time_entry;
  GtkWidget *task_day_entry;

  GtkWidget *cal_expander;

  GtkEntryBuffer *task_name_entry_buffer;
  GtkEntryBuffer *task_description_entry_buffer;

  task_name_entry = gtk_widget_get_child_by_name(GTK_WIDGET(dialog), "taskNameInput");
  task_name_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(task_name_entry));
  task_description_entry = gtk_widget_get_child_by_name(GTK_WIDGET(dialog), "taskDescriptionInput");
  task_description_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(task_description_entry));
  task_time_entry = gtk_widget_get_child_by_name(GTK_WIDGET(dialog), "timeInput");
  cal_expander = gtk_widget_get_child_by_name(GTK_WIDGET(dialog), "calendarExpander");
  task_day_entry = gtk_expander_get_child(GTK_EXPANDER(cal_expander));

  task_name = g_strdup(gtk_entry_buffer_get_text(task_name_entry_buffer));
  task_description = g_strdup(gtk_entry_buffer_get_text(task_description_entry_buffer));
  task_deadline = parse_deadline_time(GTK_SPIN_BUTTON(task_time_entry), GTK_CALENDAR(task_day_entry));

  task = task_init();
  task_set_name(task, task_name);
  task_set_description(task, task_description);
  task_set_deadline(task, task_deadline);

  return task;
}