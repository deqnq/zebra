#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "pty.h"
#include "debug.h"

int runParent(Pty *pty){
    close(pty->slave);
    sleep(5);
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
    listdev();
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
    listdev();

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
