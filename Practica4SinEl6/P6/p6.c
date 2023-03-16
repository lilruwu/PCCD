#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 100

// Declaración de variables globales
sem_t sem_leer, sem_fin_lectura, sem_dentro_l;
sem_t sem_aux_entrada_leer[20];
sem_t sem_aux_entrada_fin_l[20];
sem_t sem_escribir, sem_fin_escritura, sem_dentro_e;
sem_t sem_aux_entrada_escribir[20];
sem_t sem_aux_entrada_fin_e[20];
sem_t sem_escritor_esperando, sem_lector_esperando;
sem_t sem_exmu_papel;
int lectores_esperando, escritores_esperando, escritores_dentro, lectores_dentro;

int n1, n2, n3;

void* lector(void* arg) {
    int id = *(int*) arg;
    while (1) {
        
        lectores_esperando++;
        if (escritores_dentro > 0){
            sem_wait(&sem_lector_esperando);
        } else if(lectores_esperando != 0) {
            sem_post(&sem_lector_esperando);
        }
        printf("[Lector %d] -> Esperando a intentar leer...\n", id);
        sem_wait(&sem_aux_entrada_leer[id]); //este semaforo es para esperar a que me pidan leer
        printf("[Lector %d] -> Intentando leer...\n", id);
        sem_wait(&sem_dentro_l); //este es cuando el tio esta leyendo
        lectores_dentro++;
        sem_post(&sem_dentro_l); 
        sem_wait(&sem_lector_esperando);
        lectores_esperando--;
        sem_post(&sem_lector_esperando);
        sem_wait(&sem_exmu_papel);
        printf("[Lector %d] -> Leyendo...\n", id);
        sem_wait(&sem_aux_entrada_fin_l[id]); //este es cuando ha acabado de leer
        
        printf("[Lector %d] -> Fin lectura\n", id);
        sem_post(&sem_exmu_papel);
        sem_wait(&sem_dentro_l);
        lectores_dentro--;
        sem_post(&sem_dentro_l);
        //esto es pq el pive acabo y ya puede entrar el siguiente
        if (lectores_dentro==0 && escritores_esperando!=0) {
            sem_post(&sem_escritor_esperando);
        }
    }
    return NULL;
}


void* escritor(void* arg) {
    int ide = *(int*) arg;
    while (1) {
        
        escritores_esperando++;
        if (lectores_dentro > 0){
            sem_wait(&sem_escritor_esperando);
        } else if(escritores_esperando != 0) {
            sem_post(&sem_escritor_esperando);
        }
        printf("[Escritor %d] -> Esperando a intentar escribir...\n", ide);
        sem_wait(&sem_aux_entrada_escribir[ide]); //este semaforo es para esperar a que me pidan leer
        printf("[Escritor %d] -> Intentando escribir...\n", ide);
        sem_wait(&sem_dentro_e); //este es cuando el tio esta leyendo
        escritores_dentro++;
        sem_post(&sem_dentro_e); 
        sem_wait(&sem_escritor_esperando);
        escritores_esperando--;
        sem_post(&sem_escritor_esperando);
        sem_wait(&sem_exmu_papel);
        printf("[Escritor %d] -> Escribiendo...\n", ide);
        sem_wait(&sem_aux_entrada_fin_e[ide]); //este es cuando ha acabado de leer
        
        printf("[Escritor %d] -> Fin escritura\n", ide);
        sem_post(&sem_exmu_papel);
        sem_wait(&sem_dentro_e);
        escritores_dentro--;
        sem_post(&sem_dentro_e);
        //esto es pq el pive acabo y ya puede entrar el siguiente
        if (escritores_dentro==0 && lectores_esperando!=0) {
            sem_post(&sem_lector_esperando);
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    int i, opcion, lector_id, j, escritor_id;
    pthread_t lectores[N];
    pthread_t escritores[N];

    // Obtener argumentos
    if (argc != 4) {
        printf("Error: Debe ingresar dos argumentos enteros.\n");
        return -1;
    }
    n1 = atoi(argv[1]);
    n2 = atoi(argv[2]);
    n3 = atoi(argv[3]);

    // Inicialización de semáforos

    sem_init(&sem_dentro_l,0,n2);
    sem_init(&sem_dentro_e,0,1);
    sem_init(&sem_escritor_esperando, 0, 0);
    sem_init(&sem_lector_esperando, 0, 0);
    sem_init(&sem_exmu_papel, 0, 1);

    // Creación de hilos lectores
    for (i = 0; i < n1; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&lectores[i], NULL, lector, id);
        sem_init(&sem_leer, 0, 0);
        sem_aux_entrada_leer[i]=sem_leer;
        sem_init(&sem_fin_lectura, 0, 0);
        sem_aux_entrada_fin_l[i]=sem_fin_lectura;
    }
    

     for (j = 0; i < n3; j++) {
        int* ide = malloc(sizeof(int));
        *ide = j + 1;
        pthread_create(&escritores[j], NULL, escritor, ide);
        sem_init(&sem_escribir, 0, 0);
        sem_aux_entrada_escribir[j]=sem_escribir;
        sem_init(&sem_fin_lectura, 0, 0);
        sem_aux_entrada_fin_e[j]=sem_fin_escritura;
    }

    // Loop del thread principal
    while (1) {
        sleep(1);
        printf("Ingrese una opción:\n");
        printf("1. Intentar leer\n");
        printf("2. Finalizar leer\n");
         printf("3. Intentar escribir\n");
        printf("4. Finalizar escribir\n");
        printf("5. Salir\n");
        scanf("%d", &opcion);

        if (opcion == 1) {
            printf("Ingrese el número del lector (de 1 a %d):\n", n1);
            scanf("%d", &lector_id);
            printf("[Main] -> Lector %d tiene acceso a la lectura.\n", lector_id);
            sem_post(&sem_aux_entrada_leer[lector_id]);
        } else if (opcion == 2) {
            printf("Ingrese el número del lector (de 1 a %d):\n", n1);
            scanf("%d", &lector_id);
            printf("[Main] -> Lector %d finalizó su lectura.\n", lector_id);
            sem_post(&sem_aux_entrada_fin_l[lector_id]);
        } else if (opcion == 3) {
            
            printf("Ingrese el número del escritor (de 1 a %d):\n", n3);
            scanf("%d", &escritor_id);
            printf("[Main] -> Escritor %d solicita acceso a la escritura.\n", escritor_id);
            sem_post(&sem_aux_entrada_escribir[escritor_id]);
        } else if (opcion == 4) {
            printf("Ingrese el número del escritor (de 1 a %d):\n", n3);
            scanf("%d", &escritor_id);
            printf("[Main] -> Escritor %d finalizó su escritura.\n", escritor_id);
            sem_post(&sem_aux_entrada_fin_e[escritor_id]);

            } else if (opcion == 5) {
            break;
           
        } else {
            printf("Opción inválida.\n");
        }
    }

    // Finalización del programa
    for (i = 0; i < n1; i++) {
        pthread_cancel(lectores[i]);
    }

    for (j = 0; i < n3; j++) {
        pthread_cancel(escritores[j]);
    }


    return 0;
}
