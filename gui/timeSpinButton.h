#ifndef _TimeSpinButton_H_
#define _TimeSpinButton_H_

#include <gtk/gtk.h>

int time_spinbutton_input (GtkSpinButton *spin_button, gdouble *new_val, gpointer user_data);
int time_spinbutton_output (GtkSpinButton *spin_button, gpointer user_data);


#endif