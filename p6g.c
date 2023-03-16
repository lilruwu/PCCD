#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_READERS 5

sem_t mutex, writeblock;
int data = 0, readers = 0;

void *reader(void *arg){
    int num = *(int*) arg;
    while(1){
        sem_wait(&mutex);
        readers++;
        if(readers == 1){
            sem_wait(&writeblock);
        }
        sem_post(&mutex);
        printf("Reader %d reads data: %d\n", num, data);
        sem_wait(&mutex);
        readers--;
        if(readers == 0){
            sem_post(&writeblock);
        }
        sem_post(&mutex);
    }
}

void *writer(void *arg){
    int num = *(int*) arg;
    while(1){
        sem_wait(&writeblock);
        data++;
        printf("Writer %d writes data: %d\n", num, data);
        sem_post(&writeblock);
    }
}

int main(){
    pthread_t readers[MAX_READERS], writer1, writer2;
    int reader_args[MAX_READERS] = {1, 2, 3, 4, 5};
    int writer_args[2] = {1, 2};
    sem_init(&mutex, 0, 1);
    sem_init(&writeblock, 0, 1);
    for(int i = 0; i < MAX_READERS; i++){
        pthread_create(&readers[i], NULL, reader, &reader_args[i]);
    }
    pthread_create(&writer1, NULL, writer, &writer_args[0]);
    pthread_create(&writer2, NULL, writer, &writer_args[1]);
    for(int i = 0; i < MAX_READERS; i++){
        pthread_join(readers[i], NULL);
    }
    pthread_join(writer1, NULL);
    pthread_join(writer2, NULL);
    sem_destroy(&mutex);
    sem_destroy(&writeblock);
    return 0;
}
