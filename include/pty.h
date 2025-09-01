#ifndef PTY_H
#define PTY_H
typedef struct {
    int master;
    int slave;
} Pty;

int get_master(Pty *pty);
int get_slave(Pty *pty);
void cleanup(Pty *pty);
#endif
