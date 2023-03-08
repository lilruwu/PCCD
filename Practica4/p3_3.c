#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *funcion_hilo(void *arg) {
    int i = *((int*)arg);
    printf("Soy el hilo %d, voy a dormir\n", i);
    sleep(5);
    printf("El hilo %d ha terminado de dormir\n", i);
    int *retorno = malloc(sizeof(int));
    *retorno = i + 1;
    pthread_exit((void*)retorno);
}

int main() {
    int i;
    int hijos_finalizados = 0;

    pthread_t threads[3];
    int args[3];

    for (i = 0; i < 3; i++) {
        args[i] = i;
        if (pthread_create(&threads[i], NULL, funcion_hilo, (void*)&args[i])) {
            perror("Error al crear el hilo");
            return 1;
        }
    }

    int status;
    void *retorno;
    while (hijos_finalizados < 3) {
        for (i = 0; i < 3; i++) {
            if (pthread_tryjoin_np(threads[i], &retorno) == 0) {
                hijos_finalizados++;
                printf("El hilo %d ha finalizado su ejecución con código %d\n", i, *((int*)retorno));
                free(retorno);
            }
        }
    }
    printf("Todos los hilos han finalizado su ejecución\n");
    return 0;
}
