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