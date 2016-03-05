static void
on_certificate_parsed (GcrParser *parser, gpointer user_data)
{
	GcrCertificateWidget *certificate;
	GtkDialog *dialog = GTK_DIALOG (user_data);
	GcrCertificate *cert;
	gpointer dn;
	gsize n_dn;
	gchar *string;

	certificate = g_object_new (GCR_TYPE_CERTIFICATE_WIDGET,
	                        "attributes", gcr_parser_get_parsed_attributes (parser),
	                        NULL);

	cert = gcr_certificate_widget_get_certificate (certificate);

	

	gtk_widget_show (GTK_WIDGET (certificate));
	gtk_container_add (GTK_CONTAINER (gtk_dialog_get_content_area (dialog)), GTK_WIDGET (certificate));

	gtk_container_set_border_width (GTK_CONTAINER (dialog), 10);
}

static void
loaded_certificate (const gchar *path)
{

	gtk_init (0,NULL);
	g_set_prgname ("Loaded-Certificate");
	GcrParser *parser;
	GError *err = NULL;
	guchar *data;
	gsize n_data;
	GBytes *bytes;
	GtkWidget *dialog;

	if (!g_file_get_contents (path, (gchar**)&data, &n_data, NULL))
		g_error ("couldn't read file: %s", path);

	dialog = gtk_dialog_new ();
	g_object_ref_sink (dialog);

	parser = gcr_parser_new ();
	g_signal_connect (parser, "parsed", G_CALLBACK (on_certificate_parsed), dialog);
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
