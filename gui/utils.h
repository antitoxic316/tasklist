#ifndef UTILS_H
#define UTILS_H

#include <gtk/gtk.h>
#include <time.h>

GtkWidget *gtk_widget_get_child_by_name(GtkWidget* widget, char* name);

time_t parse_deadline(GtkSpinButton *time_entry, GtkCalendar *day_entry);
#endif