#include "mainwindow.h"

void activate_mainwindow(GtkApplication *app, gpointer user_data){
  GtkBuilder *builder = gtk_builder_new();
  gboolean ui_load_res = gtk_builder_add_from_file(builder, "gui/ui/mainwindow.ui", NULL);

  GtkCssProvider *cssParser = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssParser, "gui/css/mainwindow.css");
  gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(cssParser), GTK_STYLE_PROVIDER_PRIORITY_USER);

  if (!ui_load_res) {
    g_printerr("Error loading UI file\n");
    return;
  }

  GObject *window = gtk_builder_get_object(builder, "mainwindow");
  gtk_window_set_application (GTK_WINDOW (window), app);

  GObject *newTaskButton = gtk_builder_get_object(builder, "newTaskButton");
  GObject *removeTaskButton = gtk_builder_get_object(builder, "removeTaskButton");

  gtk_widget_set_visible (GTK_WIDGET (window), TRUE);

  g_object_unref (builder);
}