#include "editTaskDialog.h"
#include "utils.h"
#include "timeSpinButton.h"
#include "tasksboxlist.h"
#include "path_config.h"


struct _TaskappEditTaskDialog{
  GtkDialog parent;

  Task *task_ref;

  GtkWidget *taskNameInput, *taskDescriptionInput;
  GtkWidget *taskTimeInput;
  GtkWidget *taskDayInput;
};

G_DEFINE_TYPE (TaskappEditTaskDialog, taskapp_edit_task_dialog, GTK_TYPE_DIALOG);


static Task *taskapp_edit_task_dialog_parse_task(TaskappEditTaskDialog *self){
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

static void taskapp_edit_task_dialog_accept(TaskappEditTaskDialog *dialog){
    GtkWindow *mainwindow;
    GtkWidget *tasksboxlist;

    mainwindow = gtk_window_get_transient_for(GTK_WINDOW(dialog));
    tasksboxlist = gtk_widget_get_child_by_name(gtk_window_get_child(mainwindow), "tasksBoxList");

    Task *new_task = taskapp_edit_task_dialog_parse_task(dialog);

    task_replace_data(dialog->task_ref, new_task);
    task_free(new_task);
    tasksboxlist_refresh(GTK_LIST_BOX(tasksboxlist));
}

static void taskapp_edit_task_dialog_reject(TaskappEditTaskDialog *dialog){
    return;
}

static void taskapp_edit_task_dialog_response_cb(GtkDialog *dialog, gint response_id){
  if(response_id == GTK_RESPONSE_ACCEPT){
    taskapp_edit_task_dialog_accept(TASKAPP_EDIT_TASK_DIALOG(dialog));
  } else if(response_id == GTK_RESPONSE_REJECT) {
    taskapp_edit_task_dialog_reject(TASKAPP_EDIT_TASK_DIALOG(dialog));
  }
}

static void taskapp_edit_task_dialog_set_fields(TaskappEditTaskDialog *self, Task *t_ref){
  GtkEntryBuffer *task_name_entry_buffer;
  GtkEntryBuffer *task_description_entry_buffer;
  
  task_name_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(self->taskNameInput));
  task_description_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(self->taskDescriptionInput));

  /*
  task_name = g_strdup(gtk_entry_buffer_get_text(task_name_entry_buffer));
  task_description = g_strdup(gtk_entry_buffer_get_text(task_description_entry_buffer));
  task_deadline = parse_deadline_time(GTK_SPIN_BUTTON(self->taskTimeInput), GTK_CALENDAR(self->taskDayInput));
  */
}

GtkWidget *taskapp_edit_task_dialog_new(GtkWindow *parent_win, Task *t_ref){
  TaskappEditTaskDialog *dlg = g_object_new(TASKAPP_TYPE_EDIT_TASK_DIALOG, 
  NULL);

  gtk_window_set_transient_for(GTK_WINDOW(dlg), parent_win);
  
  dlg->task_ref = t_ref;
  taskapp_edit_task_dialog_set_fields(dlg, dlg->task_ref);

  return GTK_WIDGET(dlg);
}


static void taskapp_edit_task_dialog_class_init(TaskappEditTaskDialogClass *klass){
  GObjectClass *obj_class;
  GtkDialogClass *dlg_class;

  obj_class = G_OBJECT_CLASS(klass);
  dlg_class = GTK_DIALOG_CLASS(klass);

  dlg_class->response = taskapp_edit_task_dialog_response_cb;

  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/com/github/antitoxic316/gui/ui/edittaskdialog.ui");
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), TaskappEditTaskDialog, taskNameInput);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), TaskappEditTaskDialog, taskDescriptionInput);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), TaskappEditTaskDialog, taskTimeInput);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), TaskappEditTaskDialog, taskDayInput);
}


static void taskapp_edit_task_dialog_init(TaskappEditTaskDialog *self){
  gtk_widget_init_template(GTK_WIDGET(self));

  gtk_window_set_modal(GTK_WINDOW(self), TRUE);

  g_signal_connect_swapped(self,
                            "response",
                            G_CALLBACK (gtk_window_destroy),
                            self);
}