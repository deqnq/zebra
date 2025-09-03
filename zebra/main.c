#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <gtk/gtk.h>
#include <vte/vte.h>
#include "pty.h"
#include "debug.h"

int runParent(Pty *pty) {
    close(pty->slave);
    gtk_init(NULL, NULL);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Terminal Emulator");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    GtkWidget *terminal = vte_terminal_new();
    GError *error = NULL;
    VtePty *vte_pty = vte_pty_new_foreign_sync(pty->master, NULL, &error);
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

int runChild(Pty *pty){
    close(pty->master);
    // with setsid()
    // the process is the leader of the new session
    // we can use ioctl() with TIOCSCTTY
    if (!setsid()) {
        perror("setsid");
        return 1;
    }
    // pty->slave becomes the control terminal of the process
    int set = ioctl(pty->slave, TIOCSCTTY, 0);
    if (set < 0){
        perror("ioctl");
        return 1;
    }
    // redirection
    dup2(pty->slave, 0);
    dup2(pty->slave, 1);
    dup2(pty->slave, 2);
    execvp("bash", (char *[]){"bash", NULL});
    return 0;
}

int main (){
    Pty pty;
    int err = get_master(&pty);
    if (err) return 1;
    err = get_slave(&pty);
    if (err) return 1;

    // with fork()
    // the child process is not the pgroup leader
    // we can use setsid()
    pid_t pid = fork();
    if (pid == 0) err = runChild(&pty);
    else if (pid > 0) err = runParent(&pty);
    else return 1;
    if (err) return 1;

    clean_pty(&pty);
    return 0;
}
