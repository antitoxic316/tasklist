#ifndef UTILS_H
#define UTILS_H

#include <gtk/gtk.h>
#include <time.h>

GtkWidget *gtk_widget_get_child_by_name(GtkWidget* widget, char* name);

time_t parse_deadline_time(GtkSpinButton *time_entry, GtkCalendar *day_entry);
void getTimeRemainedStr(time_t deadline, char *buf, size_t n);
#endif