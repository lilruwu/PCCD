#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>

void *child_function(void *arg) {
    char *nombre = (char *)arg;
    printf("%s ", nombre);
    return NULL;
}

int main(){

    int i;
    int contador = 0;
    int auxThiago = 0;
    int finHijo = 0;
    int fin = 0;
    pthread_t threads[6];
    char *nombres[] = {"De Gea", "Carvajal", "Ramos", "Jordi Alba", "Busquets", "Isco"};

    printf("De Gea ");

    for (i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, child_function, (void *)nombres[i+1]) != 0) {
            perror("Error al crear el thread hijo");
            return 1;
        }
    }

    printf("Piqué ");

    if (pthread_create(&threads[3], NULL, child_function, (void *)"Thiago ") != 0) {
        perror("Error al crear el thread hijo");
        return 1;
    }

    if (pthread_create(&threads[4], NULL, child_function, (void *)"Silva ") != 0) {
        perror("Error al crear el thread hijo");
        return 1;
    }

    for (i = 0; i < 2; i++) {
        if (pthread_create(&threads[i+5], NULL, child_function, (void *)nombres[i+4]) != 0) {
            perror("Error al crear el thread hijo");
            return 1;
        }
    }

    for (i = 0; i < 6; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error al esperar por el thread hijo");
            return 1;
        }
    }

    printf("Morata\n");

    return 0;
}
