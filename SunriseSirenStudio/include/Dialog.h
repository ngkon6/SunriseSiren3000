#ifndef _dialog_h
#define _dialog_h

gint show_message_dialog(GtkWindow *parent, GtkMessageType type, GtkButtonsType buttons, gchar* title, gchar* text) {
    GtkMessageDialog *message_dialog = gtk_message_dialog_new(parent, GTK_DIALOG_MODAL, type, buttons, text);
    gtk_window_set_title(message_dialog, title);

    gint *response = gtk_dialog_run(message_dialog);

    gtk_widget_destroy(message_dialog);
    return response;
}

void show_about_dialog() {
    GStrvBuilder *credit_builder = g_strv_builder_new();
    g_strv_builder_add(credit_builder, "ngkon");
    GStrv *credits = g_strv_builder_end(credit_builder);

    gtk_show_about_dialog(MainWindow,
        "program-name", "Sunrise Siren Studio",
        "title", "About Sunrise Siren Studio",
        "version", STUDIO_VERSION,
        "logo-icon-name", "sunrise-siren-studio",
        "license-type", GTK_LICENSE_GPL_3_0,
        "copyright", "© 2025 ngkon. All rights reserved.",
        "authors", credits,
        "artists", credits,
        "documenters", credits,
    NULL);

    g_strfreev(credits);
}

#endif
