#include "editTaskDialog.h"
#include "utils.h"
#include "timeSpinButton.h"
#include "tasksboxlist.h"
#include "path_config.h"

#include <time.h>

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

    TaskList *old_tasks = g_object_get_data(G_OBJECT(tasksboxlist), "tasks");

    Task *new_task = taskapp_edit_task_dialog_parse_task(dialog);

    task_replace_data(dialog->task_ref, new_task);
    task_free(new_task);

    Task **tasks_array_p = NULL;
    size_t tasks_count_p = 0;
    tasklist_to_array(old_tasks, &tasks_array_p, &tasks_count_p);
    tasklist_serializer_rewrite_state(tasks_array_p, tasks_count_p);

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
  GtkCalendar *cal;
  
  task_name_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(self->taskNameInput));
  task_description_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(self->taskDescriptionInput));

  gtk_entry_buffer_set_text(task_name_entry_buffer, t_ref->name, strlen(t_ref->name));
  gtk_entry_buffer_set_text(task_description_entry_buffer, t_ref->description, strlen(t_ref->description));

  cal = GTK_CALENDAR(self->taskDayInput);

  struct tm date = *localtime(&t_ref->deadline);

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(self->taskTimeInput), date.tm_hour*date.tm_min);
  gtk_calendar_set_year(cal, date.tm_year);
  gtk_calendar_set_month(cal, date.tm_mon);
  gtk_calendar_set_day(cal, date.tm_mday);
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
  GtkWidgetClass *wdg_class;
  GtkBuilderScope *scope;

  obj_class = G_OBJECT_CLASS(klass);
  dlg_class = GTK_DIALOG_CLASS(klass);
  wdg_class = GTK_WIDGET_CLASS(klass);

  dlg_class->response = taskapp_edit_task_dialog_response_cb;

  scope = gtk_builder_cscope_new();
  gtk_builder_cscope_add_callback(scope, time_spinbutton_input);
  gtk_builder_cscope_add_callback(scope, time_spinbutton_output);

  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/com/github/antitoxic316/gui/ui/edittaskdialog.ui");
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), TaskappEditTaskDialog, taskNameInput);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), TaskappEditTaskDialog, taskDescriptionInput);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), TaskappEditTaskDialog, taskTimeInput);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(klass), TaskappEditTaskDialog, taskDayInput);

  gtk_widget_class_set_template_scope(wdg_class, scope);
}


static void taskapp_edit_task_dialog_init(TaskappEditTaskDialog *self){
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