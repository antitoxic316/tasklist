#ifndef _TimeSpinButton_H_
#define _TimeSpinButton_H_

#include <gtk/gtk.h>

int time_spinbutton_input (GtkSpinButton *spin_button, double *new_val);
int time_spinbutton_output (GtkSpinButton *spin_button);


#endif