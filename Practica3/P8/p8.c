#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <numero>\n", argv[0]);
        return -1;
    }

    // Genera la clave
    int key = ftok("keys.txt", 'L');
    if (key == -1) {
        printf("No se ha podido generar la clave\n");
        return -1;
    }
    // Crea el segmento de memoria compartida
    int id = shmget(key, 2 * sizeof(int), (0666 | IPC_CREAT));
    if (id == -1) {
        printf("No se ha podido crear el segmento\n");
        return -1;
    }

    printf("Id de la zona de memoria: %d\n", id);
    // Enlaza el segmento de memoria compartida
    int *memoria = shmat(id, (void *)0, 0);
    if (memoria == (int *)(-1)) {
        printf("No se ha podido enlazar el segmento\n");
        return -1;
    }

    int propio = atoi(argv[1]);
    int ajeno = (propio == 0) ? 1 : 0; // Identifica el otro proceso
    memoria[0] = 1;
    memoria[1] = 1;
    while (1) {
        printf("Caminando por mi habitación.\n");
        getchar();
        printf("Dentro del pasillo\n");
        getchar();
        memoria[ajeno] = 0;
        printf("He accionado el pulsador\n");
        getchar();
        printf("Intentando acceder a la sección crítica...\n");
        while (memoria[ajeno] == 0) {
            printf("Esperando a que el otro proceso salga de la sección crítica...\n");
            sleep(1);
        }
        if (memoria[propio] == 1) {
            printf("Dentro de mi sección crítica\n");
            getchar();
            printf("He salido de mi sección crítica\n");
            getchar();
            memoria[ajeno] = 1;
            printf("He accionado el pulsador\n");
        } else {
            printf("Puerta cerrada. Saliendo del pasillo...\n");
            getchar();
            memoria[ajeno] = 1;
            printf("He accionado el pulsador\n");
            printf("He salido del pasillo\n");
        }
    }
    // Desenlaza el segmento de memoria compartida
    if (shmdt(memoria) == -1) {
        printf("No se ha podido desenlazar el segmento\n");
    }
    return 0;
}

