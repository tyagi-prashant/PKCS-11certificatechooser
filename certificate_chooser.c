#define GCR_API_SUBJECT_TO_CHANGE 1
#include <gcr/gcr.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "certificate_renderer.c"

static void
certificate_chooser (GtkWidget *widget,
             gpointer   data)
{


  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Open File",
                                      data,
                                      action,
                                      ("Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("Open"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);
  GtkFileFilter *filefilter = gtk_file_filter_new();
  gtk_file_filter_set_name(filefilter,"PEM certificates(*.pem,*.crt,*.key");
  gtk_file_filter_add_pattern(filefilter,"*.pem");
  gtk_file_filter_add_pattern(filefilter,"*.crt");
  gtk_file_filter_add_pattern(filefilter,"*.key");
  gtk_file_filter_add_pattern(filefilter,"*.p12");
  GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
  gtk_file_chooser_add_filter(chooser, filefilter); 
 
  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT) {
    char *uri;
    uri = gtk_file_chooser_get_filename (chooser);
    loaded_certificate(uri);
    g_print("%s\n", uri);

    g_free (uri);
  }

  gtk_widget_destroy (dialog);

}

int
main (int   argc,
      char *argv[])
{
 
  GtkBuilder *builder;
  GObject *window;
  GObject *button;
  gtk_init (&argc, &argv);

  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "builder.ui", NULL);
  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), window);
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

  button = gtk_builder_get_object (builder, "button1");
  g_signal_connect (button, "clicked", G_CALLBACK (certificate_chooser), NULL);

  gtk_main ();
 
  return 0;
}
