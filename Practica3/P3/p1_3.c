#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]) {
    // Obtener el valor de entrada
    if (argc != 2) {
        printf("Debe proporcionar un número como argumento.\n");
        exit(1);
    }
    int valor = atoi(argv[1]);

    // Obtener un identificador único para el segmento de memoria compartida
    key_t key = ftok("/home/Practica3/keys.txt", 'L'); 
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Asociar la zona de memoria compartida con la dirección de memoria del proceso
    int *shmaddr = (int *) shmat(shmid, NULL, 0);
    if (shmaddr == (int *) -1) {
        perror("shmat");
        exit(1);
    }

    // Escribir el valor en la zona de memoria compartida en un bucle interactivo
    char c;
    do {
        *shmaddr = valor;
        printf("Se ha escrito %d en la zona de memoria compartida.\n", valor);
        printf("Presione una tecla para continuar o 'q' para salir.\n");
        scanf(" %c", &c);
    } while (c != 'q');

    // Desasociar la zona de memoria compartida de la dirección de memoria del proceso
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }

    // Marcar la zona de memoria compartida para su eliminación
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    return 0;
}
