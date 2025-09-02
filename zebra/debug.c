#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

void listdev() {
    const char *pts_path = "/dev/pts";
    DIR *dir = opendir(pts_path);

    if (!dir) {
        perror("opendir /dev/pts");
        return;
    }

    printf("Devices in /dev/pts:\n");

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

