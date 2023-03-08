#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 100

sem_t sem_leer, sem_escribir;
int n_lectores = 0;
int n_lectores_max = 0;
int lectores_esperando[N] = {0};
int lectores_leyendo[N] = {0};

void *lector(void *arg) {
    int id = *(int *)arg;
    while (1) {
        lectores_esperando[id] = 1;
        printf("[Lector %d] -> Esperando a intentar leer...\n", id);
        sem_wait(&sem_leer);
        lectores_esperando[id] = 0;

        n_lectores++;
        if (n_lectores > n_lectores_max) {
            n_lectores_max = n_lectores;
        }

        printf("[Lector %d] -> Intentando leer...\n", id);
        sem_post(&sem_leer);

        sem_wait(&sem_escribir);
        printf("[Lector %d] -> Leyendo...\n", id);
        lectores_leyendo[id] = 1;
        sem_post(&sem_escribir);

        while (lectores_leyendo[id]) {
            // Espera hasta que el thread principal indique fin de lectura
        }

        sem_wait(&sem_leer);
        n_lectores--;
        printf("[Lector %d] -> Fin lectura\n", id);
        sem_post(&sem_leer);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int i;
    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[2]);
    int id[N];

    sem_init(&sem_leer, 0, 1);
    sem_init(&sem_escribir, 0, n2);

    pthread_t threads[N];

    // Crear threads lectores
    for (i = 0; i < n1; i++) {
        id[i] = i + 1;
        pthread_create(&threads[i], NULL, lector, (void *)&id[i]);
    }

    // Thread principal
    while (1) {
        int opcion, lector;

        printf("\nMenú:\n");
        printf("1. Intentar leer\n");
        printf("2. Finalizar lectura\n");
        printf("3. Salir\n");
        printf("Ingrese una opción: ");
        scanf("%d", &opcion);

        if (opcion == 1) {
            printf("Ingrese el número del lector (1 a %d): ", n1);
            scanf("%d", &lector);

            if (lector < 1 || lector > n1) {
                printf("Número de lector inválido.\n");
            } else {
                sem_wait(&sem_leer);
                if (n_lectores >= n_lectores_max) {
                    sem_post(&sem_leer);
                    lectores_esperando[lector - 1] = 1;
                    while (lectores_esperando[lector - 1]) {
                        // Espera hasta que haya espacio para leer
                    }
                } else {
                    sem_post(&sem_leer);
                }
                // Indicar al lector que puede leer
                sem_post(&sem_escribir);
            }
        } else if (opcion == 2) {
            printf("Ingrese el número del lector (1 a %d
