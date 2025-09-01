#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "pty.h"

int get_master(Pty *pty){
    int fd = open("/dev/ptmx", O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("error: while opening /dev/ptmx.");
        return 1;
    }
    pty->master = fd;
    return 0;
}

int get_slave(Pty *pty){
    int unlock;
    int unlocked = ioctl(pty->master, TIOCSPTLCK, &unlock);
    if (unlocked < 0){
        perror("error: failed to unlock slave.");
        return 1;
    }

    int slave_index;
    int got = ioctl(pty->master, TIOCGPTN, &slave_index);
    if (got < 0){
        perror("error: failed to get the slave index.");
        return 1;
    }

    char path[20];
    snprintf(path, sizeof path, "/dev/pts/%d", slave_index);
    int fd = open(path, O_RDWR|O_NOCTTY);
	if (fd < 0){
        perror("error: failed to open slave.");
        return 1;
    }

    return 0;
}

void cleanup(Pty *pty){
    close(pty->master);
    close(pty->slave);
}
