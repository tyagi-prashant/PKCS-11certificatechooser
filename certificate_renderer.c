static void
update_preview_cb(GtkFileChooser *chooser, gpointer data)
{
    GtkWidget *certificate;
    char *filename;
    gboolean is_filename;

    filename = gtk_file_chooser_get_preview_filename (chooser);
    is_filename = (filename != NULL);
    gtk_file_chooser_set_preview_widget_active (chooser, is_filename);

    g_free(filename);

}    
static void
on_certificate_parsed (GcrParser *parser, gpointer data)
{
    GcrViewerWidget *certificate;
    certificate = GTK_WIDGET(data);
	gtk_file_chooser_set_preview_widget (chooser,
                                         certificate);
    g_signal_connect(chooser, "update-preview", G_CALLBACK(update_preview_cb), certificate);

}

static void
loaded_certificate (const gchar *path)
{

	GcrParser *parser;
    GcrViewerWidget *certificate;
	GError *err = NULL;
	guchar *data;
	gsize n_data;
	GBytes *bytes;
	GtkWidget *dialog;
        certificate = gcr_viewer_widget_new();
    GFile *file;
    file = g_file_new_for_path(path);

	if (!g_file_get_contents (path, (gchar**)&data, &n_data, NULL))
		g_error ("couldn't read file: %s", path);

    gcr_viewer_widget_load_file(certificate, file);
    parser = gcr_viewer_widget_get_parser(certificate);

	g_signal_connect (parser, "parsed", G_CALLBACK (on_certificate_parsed), certificate);
	bytes = g_bytes_new_take (data, n_data);
	if (!gcr_parser_parse_bytes (parser, bytes, &err))
		g_error ("couldn't parse data: %s", err->message);

	g_object_unref (parser);
	g_bytes_unref (bytes);

	gtk_widget_show (dialog);
	g_signal_connect (dialog, "delete-event", G_CALLBACK (gtk_main_quit), NULL);
	gtk_main ();

	g_object_unref (dialog);
}
