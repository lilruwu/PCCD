#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N_ESCRITORES 5

int contador = 0; // Variable compartida a proteger

sem_t sem_lectores, sem_escritores, mutex;

void* escritor(void* arg) {
int id = (int)arg;

while(1) {
printf("[Escritor %d] -> Esperando a intentar escribir...\n", id);
sleep(rand()%5);

sem_wait(&sem_escritores); // Espera a que no haya escritores escribiendo
sem_wait(&mutex); // Espera a que el mutex esté disponible

printf("[Escritor %d] -> Intentando escribir...\n", id);
sleep(rand()%5);

contador++; // Modifica la variable compartida
printf("[Escritor %d] -> Escribiendo... (contador = %d)\n", id, contador);
sleep(rand()%5);
printf("[Escritor %d] -> Fin escritura\n", id);

sem_post(&mutex); // Libera el mutex
sem_post(&sem_escritores); // Libera el semáforo de escritores
sleep(rand()%5);

}
}

void* lector(void* arg) {
int id = (int)arg;

while(1) {
printf("[Lector %d] -> Esperando a intentar leer...\n", id);
sleep(rand()%5);

sem_wait(&sem_lectores); // Espera a que no haya escritores escribiendo
sem_wait(&mutex); // Espera a que el mutex esté disponible

sem_post(&mutex); // Libera el mutex
sem_post(&sem_lectores); // Libera el semáforo de escritores
sleep(rand()%5);

}
}

int main() {
// Inicializar semáforos
sem_init(&sem_lectores, 0, 0);
sem_init(&sem_escritores, 0, N_ESCRITORES);
sem_init(&mutex, 0, 1);

// Crear threads escritores
pthread_t threads_escritores[N_ESCRITORES];
int ids_escritores[N_ESCRITORES];

for (int i = 0; i < N_ESCRITORES; i++) {
ids_escritores[i] = i+1;
pthread_create(&threads_escritores[i], NULL, escritor, &ids_escritores[i]);
}

// Crear threads lectores

// Esperar a que terminen los threads
for (int i = 0; i < N_ESCRITORES; i++) {
pthread_join(threads_escritores[i], NULL);
}

// Destruir semáforos
sem_destroy(&sem_lectores);
sem_destroy(&sem_escritores);
sem_destroy(&mutex);

return 0;
}
