#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main(int argc, char *argv[]) {
    bool *puerta = NULL;
    int id, key;
    int proceso = atoi(argv[1]);
    key = ftok("keys.txt", 'L');
    if (key == -1) {
        printf("No se ha podido generar la clave\n");
        return -1;
    }
    id = shmget(key, sizeof(bool), (0666|IPC_CREAT));
    if (id == -1) {
        printf("No se ha podido crear el segmento\n");
        return -1;
    }
    printf("Id de la zona de memoria: %d\n", id);
    puerta = shmat(id, NULL, 0);
    if (puerta == (bool *)(-1)) {
        printf("No se ha podido enlazar el segmento\n");
        return -1;
    } else {
        *puerta = false;
        while (true) {
            printf("Caminando por mi habitación.\n");
            getchar();
            printf("Intentando entrar en mi sección crítica...\n");
            while (*puerta != proceso) {
                printf("Puerta cerrada\n");
                getchar();
            }
            printf("Dentro de mi sección crítica\n");
            getchar();
            printf("He salido de mi sección crítica\n");
            getchar();
            *puerta = !(*puerta);
            printf("He accionado el pulsador\n");
        }
    }
    if (shmdt(puerta) == -1) {
        printf("No se ha podido desenlazar el segmento\n");
    }
    return 0;
}
