#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    // Obtener el ID del segmento de memoria compartida creado por el proceso p1
    key_t key = ftok("/home/Practica3/keys.txt", 'L'); // Identificador único del segmento
    int shmid = shmget(key, sizeof(int), 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Asociar el segmento de memoria compartida con la dirección de memoria del proceso
    int *shmaddr = (int *) shmat(shmid, NULL, 0);
    if (shmaddr == (int *) -1) {
        perror("shmat");
        exit(1);
    }

    // Leer el valor de la zona de memoria compartida y mostrarlo en pantalla
    printf("El valor de la zona de memoria compartida es %d\n", *shmaddr);

    // Desasociar el segmento de memoria compartida de la dirección de memoria del proceso
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
/*El proceso p2 obtiene el ID del segmento de memoria compartida creado por el proceso p1 con shmget y lo asocia con la dirección de memoria del proceso con shmat. 
Luego, lee el valor en la zona de memoria compartida y lo muestra en pantalla. Por último, el proceso desasocia el segmento de memoria compartida con shmdt.*/
