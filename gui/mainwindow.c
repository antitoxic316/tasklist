#include "mainwindow.h"

#include "config.h"

void activate_mainwindow(GtkApplication *app, gpointer user_data){
  GtkBuilder *builder = gtk_builder_new();
  gboolean ui_load_res = gtk_builder_add_from_file(builder, MAINWINDOW_UI_FILE_PATH, NULL);

  GtkCssProvider *cssParser = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssParser, MAINWINDOW_CSS_FILE_PATH);
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