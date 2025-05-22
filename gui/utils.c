#include "utils.h"

GtkWidget *gtk_widget_get_child_by_name(GtkWidget* widget, char* name){
  GList *parent_widgets = NULL;
  parent_widgets = g_list_append(parent_widgets, widget);

  while(g_list_length(parent_widgets)){
    GList *child_widgets = NULL;
    for(GList *current_g_list_elem = parent_widgets; 
        current_g_list_elem != NULL;
        current_g_list_elem = current_g_list_elem->next){
      GtkWidget *current_widget = current_g_list_elem->data;

  /*
      printf("%s\n", gtk_widget_get_name(current_widget));
      printf("%s\n", name);
*/
      if(!g_strcmp0(gtk_widget_get_name(current_widget), name)){
        g_list_free(parent_widgets);
        g_list_free(child_widgets);
        return current_widget;
      }

      GtkWidget *first_child = gtk_widget_get_first_child(current_widget);
      if(!first_child){
        continue;
      }
      child_widgets = g_list_append(child_widgets, first_child);
      GtkWidget *next_sibling = first_child;
      while(GTK_IS_WIDGET(next_sibling)){
        child_widgets = g_list_append(child_widgets, next_sibling);
        next_sibling = gtk_widget_get_next_sibling(next_sibling);
      }
    }

    g_list_free(parent_widgets);
    parent_widgets = g_list_copy(child_widgets);
    g_list_free(child_widgets);
  }

  g_list_free(parent_widgets);
  return NULL;
}

time_t parse_deadline(GtkSpinButton *time_entry, GtkCalendar *cal_entry){
  time_t now = time(0);
  
  struct tm time_s = *localtime(&now);

  GtkAdjustment *adj;
  double value;

  adj = gtk_spin_button_get_adjustment(time_entry);
  value = gtk_adjustment_get_value(adj);
  int hours = value / 60;
  int mins = value - hours*60;

  printf("%d %d  %d %d %d\n", hours, mins, gtk_calendar_get_day(cal_entry), gtk_calendar_get_month(cal_entry), gtk_calendar_get_year(cal_entry));

  time_s.tm_min   = mins;
  time_s.tm_hour  = hours;
  time_s.tm_mday  = gtk_calendar_get_day(cal_entry);
  time_s.tm_mon   = gtk_calendar_get_month(cal_entry);
  time_s.tm_year  = gtk_calendar_get_year(cal_entry) - 1900;

  time_s.tm_isdst = -1;

  return mktime(&time_s);
}