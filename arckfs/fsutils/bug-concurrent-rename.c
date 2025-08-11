#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *rename1(void *arg) {
    rename("/sufs/1", "/sufs/a/b/1");
    perror("rename1");
    return NULL;
}

void *rename2(void *arg) {
    rename("/sufs/a/", "/sufs/1/2/a");
    perror("rename2");
    return NULL;
}

int main() {
    pthread_t t1, t2;
    mkdir("/sufs/1", 0755);
    mkdir("/sufs/a", 0755);
    mkdir("/sufs/a/b", 0755);
    mkdir("/sufs/1/2", 0755);
    pthread_create(&t1, NULL, rename1, NULL);
    pthread_create(&t2, NULL, rename2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}