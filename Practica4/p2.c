#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_thread(void *arg) {
    int pos = *((int *) arg);
    char *str = *((char **) (arg + sizeof(int)));
    printf("Thread %d: %s\n", pos, str);
    return NULL;
}

int main(int argc, char *argv[]) {
    int i, err;
    pthread_t *tids;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <string1> <string2> ... <stringN>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    tids = (pthread_t *) malloc(sizeof(pthread_t) * (argc - 1));
    if (tids == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < argc - 1; i++) {
        void *arg = malloc(sizeof(int) + sizeof(char *));
        if (arg == NULL) {
            fprintf(stderr, "Error allocating memory\n");
            exit(EXIT_FAILURE);
        }
        *((int *) arg) = i + 1;
        *((char **) (arg + sizeof(int))) = argv[i + 1];
        err = pthread_create(&tids[i], NULL, print_thread, arg);
        if (err != 0) {
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < argc - 1; i++) {
        pthread_join(tids[i], NULL);
    }

    free(tids);
    return 0;
}
