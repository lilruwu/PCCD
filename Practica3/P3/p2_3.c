#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    // Obtener el identificador único de la zona de memoria compartida creada por p1
    key_t key = ftok("/home/Practica3/keys.txt", 'L');
    int shmid = shmget(key, sizeof(int), 0666);
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

    // Leer el valor de la zona de memoria compartida en un bucle interactivo
    char c;
    do {
        printf("El valor de la zona de memoria compartida es %d.\n", *shmaddr);
        printf("Presione una tecla para continuar o 'q' para salir.\n");
        scanf(" %c", &c);
    } while (c != 'q');

    // Desasociar la zona de memoria compartida de la dirección de memoria del proceso
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
