#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("/sufs/bug", O_CREAT | O_WRONLY | O_EXCL, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    // dprintf(fd, "This is file %s\n", path);
    close(fd);
}