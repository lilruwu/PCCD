#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CHARS 10000

int num_chars = 0;

void *keyboard_thread(void *arg) {
    char c;
    while ((c = getchar()) != 'q') {
        num_chars++;
    }
    return NULL;
}

int main() {
    pthread_t tid;
    int err;

    err = pthread_create(&tid, NULL, keyboard_thread, NULL);
    if (err != 0) {
        fprintf(stderr, "Error creating thread\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Total characters read: %d\n", num_chars);
        sleep(1);
    }

    pthread_join(tid, NULL);
    return 0;
}
