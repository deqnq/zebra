#include <stdio.h>
#include <pty.h>

int main (){
    Pty pty;
    int err = get_master(&pty);
    if (err) return 1;
    err = get_slave(&pty);
    if (err) return 1;
    printf("%d, %d\n", pty.master, pty.slave);

    Pty pty2;
    err = get_master(&pty2);
    if (err) return 1;
    err = get_slave(&pty2);
    if (err) return 1;
    printf("%d, %d\n", pty2.master, pty2.slave);

    clean_pty(&pty);
    clean_pty(&pty2);
    return 0;
}

