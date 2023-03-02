#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    // Obtener el identificador único de la zona de memoria compartida creada por p1
    key_t key = ftok("keys.txt", 'L');
    int shmid = shmget(key, sizeof(int), 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Liberar la zona de memoria compartida
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    return 0;
}
