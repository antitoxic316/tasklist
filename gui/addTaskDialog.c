#include "addTaskDialog.h"
#include "utils.h"
#include "timeSpinButton.h"
#include "tasksboxlist.h"
#include "path_config.h"


struct _TaskappAddTaskDialog{
  GtkDialog parent;

  Task *task_ref;

  GtkWidget *taskNameInput, *taskDescriptionInput;
  GtkWidget *taskTimeInput;
  GtkWidget *taskDayInput;
};

G_DEFINE_TYPE (TaskappAddTaskDialog, taskapp_add_task_dialog, GTK_TYPE_DIALOG);


static Task *taskapp_add_task_dialog_parse_task(TaskappAddTaskDialog *self){
  GtkEntryBuffer *task_name_entry_buffer;
  GtkEntryBuffer *task_description_entry_buffer;
  
  Task *task;
  char *task_name;
  char *task_description;
  time_t task_deadline;

  task_name_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(self->taskNameInput));
  task_description_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(self->taskDescriptionInput));

  task_name = g_strdup(gtk_entry_buffer_get_text(task_name_entry_buffer));
  task_description = g_strdup(gtk_entry_buffer_get_text(task_description_entry_buffer));
  task_deadline = parse_deadline_time(GTK_SPIN_BUTTON(self->taskTimeInput), GTK_CALENDAR(self->taskDayInput));

  task = task_init();
  task_set_name(task, task_name);
  task_set_description(task, task_description);
  task_set_deadline(task, task_deadline);

  return task;
}

static void taskapp_add_task_dialog_accept(TaskappAddTaskDialog *dialog){
    GtkWindow *mainwindow;
    GtkWidget *tasksboxlist;

    mainwindow = gtk_window_get_transient_for(GTK_WINDOW(dialog));
    tasksboxlist = gtk_widget_get_child_by_name(gtk_window_get_child(mainwindow), "tasksBoxList");

    TaskList *app_tasks;
    app_tasks = g_object_get_data(G_OBJECT(tasksboxlist), "tasks");

    Task *new_task = taskapp_add_task_dialog_parse_task(dialog);

    app_tasks = tasklist_append(app_tasks, new_task);
    g_object_set_data(G_OBJECT(tasksboxlist), "tasks", app_tasks);
    tasksboxlist_refresh(GTK_LIST_BOX(tasksboxlist));
}

static void taskapp_add_task_dialog_reject(TaskappAddTaskDialog *dialog){
    return;
}

static void taskapp_add_task_dialog_response_cb(GtkDialog *dialog, gint response_id){
  if(response_id == GTK_RESPONSE_ACCEPT){
    taskapp_add_task_dialog_accept(TASKAPP_ADD_TASK_DIALOG(dialog));
  } else if(response_id == GTK_RESPONSE_REJECT) {
    taskapp_add_task_dialog_reject(TASKAPP_ADD_TASK_DIALOG(dialog));
  }
}

GtkWidget *taskapp_add_task_dialog_new(GtkWindow *parent_win){
  TaskappAddTaskDialog *dlg = g_object_new(TASKAPP_TYPE_ADD_TASK_DIALOG, 
  NULL);

  gtk_window_set_transient_for(GTK_WINDOW(dlg), parent_win);

  return GTK_WIDGET(dlg);
}


static void taskapp_add_task_dialog_class_init(TaskappAddTaskDialogClass *klass){
  GObjectClass *obj_class;
  GtkDialogClass *dlg_class;
  GtkWidgetClass *wdg_class;
  GtkBuilderScope *scope;

  obj_class = G_OBJECT_CLASS(klass);
  dlg_class = GTK_DIALOG_CLASS(klass);
  wdg_class = GTK_WIDGET_CLASS(klass);

  dlg_class->response = taskapp_add_task_dialog_response_cb;

  scope = gtk_builder_cscope_new();
  gtk_builder_cscope_add_callback(scope, time_spinbutton_input);
  gtk_builder_cscope_add_callback(scope, time_spinbutton_output);

  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/com/github/antitoxic316/gui/ui/addtaskdialog.ui");
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), TaskappAddTaskDialog, taskNameInput);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), TaskappAddTaskDialog, taskDescriptionInput);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), TaskappAddTaskDialog, taskTimeInput);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), TaskappAddTaskDialog, taskDayInput);

  gtk_widget_class_set_template_scope(wdg_class, scope);
}

static void taskapp_add_task_dialog_init(TaskappAddTaskDialog *self){
  gtk_widget_init_template(GTK_WIDGET(self));
  
  GtkSpinButton *time_spinbutton;
  GtkAdjustment *adjustment;

  time_spinbutton = GTK_SPIN_BUTTON(self->taskTimeInput);
  adjustment = gtk_adjustment_new(1439.0, 0.0, 1439.0, 15.0, 60.0, 0.0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(time_spinbutton), adjustment);

  gtk_window_set_modal(GTK_WINDOW(self), TRUE);

  g_signal_connect_swapped(self,
                            "response",
                            G_CALLBACK (gtk_window_destroy),
                            self);
}