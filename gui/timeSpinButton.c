#include "timeSpinButton.h"

#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>

/*
  time_spinbutton_input and time_spinbutton_output 
  functions were taken from gtk tutorial examples
*/

int time_spinbutton_input (GtkSpinButton *spin_button, gdouble *new_val, gpointer user_data)
{
  const char *text;
  char **str;
  gboolean found = FALSE;
  int hours;
  int minutes;
  char *endh;
  char *endm;

  text = gtk_editable_get_text (GTK_EDITABLE (spin_button));
  str = g_strsplit (text, ":", 2);

  if (g_strv_length (str) == 2)
    {
      hours = strtol (str[0], &endh, 10);
      minutes = strtol (str[1], &endm, 10);
      if (!*endh && !*endm &&
          0 <= hours && hours < 24 &&
          0 <= minutes && minutes < 60)
        {
          *new_val = hours * 60 + minutes;
          found = TRUE;
        }
    }

  g_strfreev (str);

  if (!found)
    {
      *new_val = 0.0;
      return GTK_INPUT_ERROR;
    }

  return TRUE;
}

int time_spinbutton_output (GtkSpinButton *spin_button, gpointer user_data)
{
  GtkAdjustment *adjustment;
  char *buf;
  double hours;
  double minutes;

  adjustment = gtk_spin_button_get_adjustment (spin_button);
  hours = gtk_adjustment_get_value (adjustment) / 60.0;
  minutes = (hours - floor (hours)) * 60.0;
  buf = g_strdup_printf ("%02.0f:%02.0f", floor (hours), floor (minutes + 0.5));
  if (strcmp (buf, gtk_editable_get_text (GTK_EDITABLE (spin_button))))
    gtk_editable_set_text (GTK_EDITABLE (spin_button), buf);
  g_free (buf);

  return TRUE;
}