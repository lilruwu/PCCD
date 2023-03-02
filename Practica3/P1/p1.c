#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]) {
    int num = atoi(argv[1]); // Leer el número de la línea de comandos

    // Crear un segmento de memoria compartida para un entero con shmget
    key_t key = ftok("keys.txt", 'L'); // Identificador único del segmento
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666); //shmid será la variable correspondiente
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Asociar el segmento de memoria compartida con la dirección de memoria del proceso
    int *shmaddr = (int *) shmat(shmid, NULL, 0); //usamos shmat para asociar la variable shmid
    if (shmaddr == (int *) -1) {
        perror("shmat");
        exit(1);
    }

    // Escribir el número en la zona de memoria compartida
    *shmaddr = num;

    // Desasociar el segmento de memoria compartida de la dirección de memoria del proceso
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }

    printf("El número %d se ha escrito en la zona de memoria compartida con ID %d\n", num, shmid);

    return 0;
}

/*El proceso p1 crea un segmento de memoria compartida con shmget y lo asocia con la dirección de memoria del proceso con shmat. 
Luego, escribe el número recibido como argumento en la zona de memoria compartida y desasocia el segmento de memoria compartida con shmdt. 
Por último, el proceso imprime un mensaje que indica que el número se ha escrito correctamente en la zona de memoria compartida.*/
