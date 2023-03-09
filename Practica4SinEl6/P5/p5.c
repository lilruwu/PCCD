#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 100

// Declaración de variables globales
sem_t sem_leer, sem_fin_lectura, sem_dentro;
sem_t sem_aux_entrada_leer[20];
sem_t sem_aux_entrada_fin[20];
int n1, n3;

// Función que ejecutará cada hilo lector
void* lector(void* arg) {
    int id = *(int*) arg;
    while (1) {
        printf("[Escritor %d] -> Esperando a intentar escribir...\n", id);
        sem_wait(&sem_aux_entrada_leer[id]); //este semaforo es para esperar a que me pidan leer
        printf("[Escritor %d] -> Intentando escribir...\n", id);
        sem_wait(&sem_dentro); //este es cuando el tio esta leyendo
        
        printf("[Escritor %d] -> Escribiendo...\n", id);
        sem_wait(&sem_aux_entrada_fin[id]); //este es cuando ha acabado de leer
        printf("[Escritor %d] -> Fin escritura\n", id);
        sem_post(&sem_dentro); //esto es pq el pive acabo y ya puede entrar el siguiente
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    int i, opcion, lector_id;
    pthread_t lectores[N];

    // Obtener argumentos
    if (argc != 2) {
        printf("Error: Debe ingresar dos argumentos enteros.\n");
        return -1;
    }
    
    n3 = atoi(argv[1]);

    // Inicialización de semáforos

    sem_init(&sem_dentro,0,n3);

    // Creación de hilos lectores
    for (i = 0; i < n3; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&lectores[i], NULL, lector, id);
        sem_init(&sem_leer, 0, 0);
        sem_aux_entrada_leer[i]=sem_leer;
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
            printf("Ingrese el número del escritor (de 1 a %d):\n", n3);
            scanf("%d", &lector_id);
            printf("[Main] -> Escritor %d tiene acceso a escribir.\n", lector_id);
            sem_post(&sem_aux_entrada_leer[lector_id]);
        } else if (opcion == 2) {
            printf("Ingrese el número del escritor (de 1 a %d):\n", n3);
            scanf("%d", &lector_id);
            printf("[Main] -> Escritor %d finalizó su escritura.\n", lector_id);
            sem_post(&sem_aux_entrada_fin[lector_id]);
        } else if (opcion == 3) {
            break;
        } else {
            printf("Opción inválida.\n");
        }
    }

    // Finalización del programa
    for (i = 0; i < n3; i++) {
        pthread_cancel(lectores[i]);
    }

    return 0;
}
