#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *funcion_hilo(void *arg) {
    char **argv = (char **)arg;
    printf("Soy el hilo, imprimiendo la lista de parámetros...\n");
    execvp(argv[0], argv);
    perror("Error al invocar a execvp");
    pthread_exit((void*)1);
}

int main(int argc, char *argv[]) {
    int i;
    int hijos_finalizados = 0;
    pthread_t threads[3];
    int status;

    for (i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, funcion_hilo, (void*)argv)) {
            perror("Error al crear el hilo");
            return 1;
        }
    }

    // Proceso padre
    void *retorno;
    while (hijos_finalizados < 3) {
        for (i = 0; i < 3; i++) {
            if (pthread_tryjoin_np(threads[i], &retorno) == 0) {
                hijos_finalizados++;
                if ((int)retorno == 0) {
                    printf("El hilo %d ha finalizado su ejecución correctamente\n", i);
                } else {
                    printf("El hilo %d ha finalizado su ejecución con errores\n", i);
                }
            }
        }
    }

    printf("Todos los hilos han finalizado su ejecución\n");
    return 0;
}
