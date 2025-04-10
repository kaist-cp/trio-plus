#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_PATH "/sufs/bug"

void *create_file(void *arg) {
    int fd = open(FILE_PATH, O_CREAT | O_WRONLY, 0777);
    if (fd == -1) {
        perror("open (create) failed");
    } else {
        printf("File created: %s\n", FILE_PATH);
        close(fd);
    }
    return NULL;
}

void *delete_file(void *arg) {
    usleep(100);
    if (unlink(FILE_PATH) == -1) {
        perror("unlink failed");
    } else {
        printf("File deleted: %s\n", FILE_PATH);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, create_file, NULL);
    pthread_create(&t2, NULL, delete_file, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}