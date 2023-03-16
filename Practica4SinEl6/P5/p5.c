#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 100

// Declaración de variables globales
sem_t sem_escribir, sem_fin_lectura, sem_dentro;
sem_t sem_aux_entrada_escribir[20];
sem_t sem_aux_entrada_fin[20];
int n1, n2;

// Función que ejecutará cada hilo escritor
void* escritor(void* arg) {
    int id = *(int*) arg;
    while (1) {
        printf("[escritor %d] -> Esperando a intentar escribir...\n", id);
        sem_wait(&sem_aux_entrada_escribir[id]); //este semaforo es para esperar a que me pidan escribir
        printf("[escritor %d] -> Intentando escribir...\n", id);
        sem_wait(&sem_dentro); //este es cuando el tio esta Escribiendo
        
        printf("[escritor %d] -> Escribiendo...\n", id);
        sem_wait(&sem_aux_entrada_fin[id]); //este es cuando ha acabado de escribir
        printf("[escritor %d] -> Fin lectura\n", id);
        sem_post(&sem_dentro); //esto es pq el pive acabo y ya puede entrar el siguiente
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    int i, opcion, escritor_id;
    pthread_t escritores[N];

    // Obtener argumentos
    if (argc != 2) {
        printf("Error: Debe ingresar dos argumentos enteros.\n");
        return -1;
    }
    n1 = atoi(argv[1]);
    n2 = 1;

    // Inicialización de semáforos

    sem_init(&sem_dentro,0,n2);

    // Creación de hilos escritores
    for (i = 0; i < n1; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&escritores[i], NULL, escritor, id);
        sem_init(&sem_escribir, 0, 0);
        sem_aux_entrada_escribir[i]=sem_escribir;
        sem_init(&sem_fin_lectura, 0, 0);
        sem_aux_entrada_fin[i]=sem_fin_lectura;
    }

    // Loop del thread principal
    while (1) {
        sleep(1);
        printf("Ingrese una opción:\n");
        printf("1. Intentar escribir\n");
        printf("2. Finalizar escribir\n");
        printf("3. Salir\n");
        scanf("%d", &opcion);
        if (opcion == 1) {
            printf("Ingrese el número del escritor (de 1 a %d):\n", n1);
            scanf("%d", &escritor_id);
            printf("[Main] -> escritor %d tiene acceso a la lectura.\n", escritor_id);
            sem_post(&sem_aux_entrada_escribir[escritor_id]);
        } else if (opcion == 2) {
            printf("Ingrese el número del escritor (de 1 a %d):\n", n1);
            scanf("%d", &escritor_id);
            printf("[Main] -> escritor %d finalizó su lectura.\n", escritor_id);
            sem_post(&sem_aux_entrada_fin[escritor_id]);
        } else if (opcion == 3) {
            break;
        } else {
            printf("Opción inválida.\n");
        }
    }

    // Finalización del programa
    for (i = 0; i < n1; i++) {
        pthread_cancel(escritores[i]);
    }

    return 0;
}
