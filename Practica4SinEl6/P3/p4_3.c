#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *carvajal(void *arg) {
    printf("Carvajal ");
    pthread_exit(NULL);
}

void *ramos(void *arg) {
    printf("Ramos ");
    pthread_exit(NULL);
}

void *jordi_alba(void *arg) {
    printf("Jordi Alba ");
    printf("Thiago ");
    printf("Silva ");
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[3];
    int contador = 0;
    int fin = 0;

    printf("De Gea ");

    pthread_create(&threads[0], NULL, carvajal, NULL);
    pthread_create(&threads[1], NULL, ramos, NULL);
    pthread_create(&threads[2], NULL, jordi_alba, NULL);

    printf("Piqué ");

    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
        contador++;
    }

    printf("Busquets ");
    printf("Isco ");
    printf("Aspas ");

    printf("Morata\n");

    return 0;
}
