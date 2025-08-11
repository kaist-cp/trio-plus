#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_PATH "/sufs/bug2"

void *read_file(void *arg) {
    sleep(1);
    int fd = open(FILE_PATH, O_WRONLY, 0777);
    if (fd == -1) {
        perror("open failed");
    } else {
        printf("Open success: %s\n", FILE_PATH);
        close(fd);
    }
    return NULL;
}

void *delete_file(void *arg) {
    if (unlink(FILE_PATH) == -1) {
        perror("unlink failed");
    } else {
        printf("File deleted: %s\n", FILE_PATH);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    int fd = open(FILE_PATH, O_CREAT | O_WRONLY, 0777);
    close(fd);
    fd = open("sufs/tmp", O_CREAT | O_WRONLY, 0777);
    close(fd);

    usleep(100);

    pthread_create(&t1, NULL, read_file, NULL);
    pthread_create(&t2, NULL, delete_file, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}