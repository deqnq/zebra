#include <gtk/gtk.h>
#include <vte/vte.h>

int runGUI(int fd){
    gtk_init(NULL, NULL);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Terminal Emulator");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    GtkWidget *terminal = vte_terminal_new();
    GError *error = NULL;
    VtePty *vte_pty = vte_pty_new_foreign_sync(fd, NULL, &error);
    if (!vte_pty) {
        g_printerr("Failed to create VtePty: %s\n", error ? error->message : "unknown error");
        g_clear_error(&error);
        return 1;
    }
    vte_terminal_set_pty(VTE_TERMINAL(terminal), vte_pty);
    gtk_container_add(GTK_CONTAINER(window), terminal);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
