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
int lectores_esperando = 0;
int escritores_esperando = 0; 
int escritores_dentro = 0; 
int lectores_dentro = 0;

int n1, n2, n3;

// Función que ejecutará cada hilo lector
void* lector(void* arg) {
    int id = *(int*) arg;
    while (1) {
        //lectores_esperando++;
        printf("[Lector %d] -> Esperando a intentar leer...\n", id);
        sem_wait(&sem_lector_esperando); //espera a que todos los escritores hayan terminado
        sem_wait(&sem_aux_entrada_leer[id]); //este semaforo es para esperar a que me pidan leer
        sem_post(&sem_lector_esperando); //deja libre el acceso para los escritores
        printf("[Lector %d] -> Intentando leer...\n", id);
        sem_wait(&sem_dentro_l); //este es cuando el tio esta leyendo       
        printf("[Lector %d] -> Leyendo...\n", id);
        sem_wait(&sem_aux_entrada_fin_l[id]); //este es cuando ha acabado de leer
        printf("[Lector %d] -> Fin lectura\n", id);
        sem_post(&sem_dentro_l); //esto es pq el pive acabo y ya puede entrar el siguiente
    }
    return NULL;
}

// Función que ejecutará cada hilo escritor
void* escritor(void* arg) {
    int id = *(int*) arg;
    while (1) {
        
        printf("[escritor %d] -> Esperando a intentar escribir...\n", id);
        sem_wait(&sem_escritor_esperando); //espera a que no haya lectores ni escritores dentro
        sem_wait(&sem_aux_entrada_escribir[id]); //este semaforo es para esperar a que me pidan escribir
        sem_post(&sem_escritor_esperando); //deja libre el acceso para los lectores y otros escritores
        printf("[escritor %d] -> Intentando escribir...\n", id);
        sem_wait(&sem_dentro_e); //este es cuando el tio esta Escribiendo       
        printf("[escritor %d] -> Escribiendo...\n", id);
        sem_wait(&sem_aux_entrada_fin_e[id]); //este es cuando ha acabado de escribir
        printf("[escritor %d] -> Fin escritura\n", id);
        sem_post(&sem_dentro_e); //esto es pq el pive acabo y ya puede entrar el siguiente
    }
    return NULL;
}

int main(int argc, char* argv[]){
    int seleccion;
    int i, opcion, lector_id;
    pthread_t lectores[N];
    int j, escritor_id;
    pthread_t escritores[N];

    // Obtener argumentos
    if (argc != 4) {
        printf("Error: Debe ingresar tres argumentos enteros.\n");
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

    // Creación de hilos escritores
    for (j = 0; j < n3; j++) {
        int* id = malloc(sizeof(int));
        *id = j + 1;
        pthread_create(&escritores[j], NULL, escritor, id);
        sem_init(&sem_escribir, 0, 0);
        sem_aux_entrada_escribir[j]=sem_escribir;
        sem_init(&sem_fin_escritura, 0, 0);
        sem_aux_entrada_fin_e[j]=sem_fin_escritura;
    }

    while (1) {
        sleep(1);
        printf("Ingrese una opción:\n");
        printf("1. Intentar leer\n");
        printf("2. Finalizar leer\n");
        printf("3. Intentar escribir\n");
        printf("4. Finalizar escribir\n");
        printf("5. Salir\n");
        scanf("%i", &opcion);
        //printf("%i", seleccion);
        //printf("%i", lectores_dentro);
        //printf("%i", escritores_esperando);
        //opcion=1;

        if (opcion == 1) {
            
            if (lectores_dentro == n2 || escritores_esperando > 0) {
                lectores_esperando++;
                sem_wait(&sem_lector_esperando);
                printf("No se puede entrar, la sala está llena o hay escritores esperando\n");          
            } if (lectores_esperando!=0) {
                sem_post(&sem_lector_esperando);
                } else {
                lectores_dentro++;
                lectores_esperando--;
                printf("Ingrese el número del lector (de 1 a %d):\n", n1);
                scanf("%d", &lector_id);
                printf("[Main] -> Lector %d tiene acceso a la lectura.\n", lector_id);
                sem_post(&sem_aux_entrada_leer[lector_id]);
            }
            } else if (opcion == 2) {
            printf("Ingrese el número del lector (de 1 a %d):\n", n1);
            scanf("%d", &lector_id);
            lectores_dentro--;
            printf("[Main] -> Lector %d finalizó su lectura.\n", lector_id);
            sem_post(&sem_aux_entrada_fin_l[lector_id]);
            if ((lectores_dentro==0) &&(escritores_esperando!=0)){
                sem_post(&sem_escritor_esperando);
            } else {
                sem_post(&sem_lector_esperando);
            }

            } else if (opcion == 3) {
               // printf("Hola");
                
            if (escritores_dentro == 1 || lectores_esperando > 0) {
                printf("No se puede entrar, la sala está llena o hay lectores en la sala\n");
                escritores_esperando++;
                sem_wait(&sem_escritor_esperando);
                
            } if (escritores_esperando!=0) {
                sem_post(&sem_escritor_esperando);
                } else {
                escritores_dentro++;
                escritores_esperando--;
                printf("Ingrese el número del escritor (de 1 a %d):\n", n1);
                scanf("%d", &escritor_id);
                printf("[Main] -> escritor %d tiene acceso a la escritura.\n", escritor_id);
                sem_post(&sem_aux_entrada_escribir[escritor_id]);
            }
            } else if (opcion == 4) {
            printf("Ingrese el número del escritor (de 1 a %d):\n", n1);
            scanf("%d", &escritor_id);
            escritores_dentro--;
            printf("[Main] -> Lector %d finalizó su lectura.\n", escritor_id);
            sem_post(&sem_aux_entrada_fin_e[escritor_id]);
            if ((escritores_dentro==0) &&(lectores_esperando!=0)){
                sem_post(&sem_lector_esperando);
            } else {
                sem_post(&sem_escritor_esperando);
            }
            
            } else if (opcion == 5) {
                break;
            }  else {
            printf("Opción inválida.\n");
        }
        }
    }

