#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <string.h>

#define KEY_FILE "keys.txt"
#define KEY_CHAR 'L'

int main(int argc, char *argv[]) {
    int key = ftok(KEY_FILE, KEY_CHAR);
    if(key == -1) {
        printf("No se ha podido generar la clave: %s\n", strerror(errno));
        return -1;
    }
    int id = shmget(key, 2*sizeof(int), (0666|IPC_CREAT));
    if(id == -1) {
        printf("No se ha podido crear el segmento: %s\n", strerror(errno));
        return -1;
    }

    printf("Id de la zona de memoria: %d\n", id);
    int *memoria = shmat(id, (void *)0, 0);
    if(memoria == (int *)(-1)) {
        printf("No se ha podido enlazar el segmento: %s\n", strerror(errno));
        return -1;
    }
    else {
        int propio = atoi(argv[1]);
        int ajeno;
        if(propio == 0) ajeno = 1;
        else ajeno = 0;
        memoria[0] = 1;
        memoria[1] = 1;
        while(1) {
            printf("Caminando por mi habitación.\n");
            getchar();
            printf("Dentro del pasillo\n");
            getchar();
            memoria[ajeno] = 0;
            printf("He accionado el pulsador\n");
            getchar();
            printf("Intentando acceder a la sección crítica...\n");
            if(memoria[propio] == 1) {
                printf("Dentro de mi sección crítica\n");
                getchar();
                printf("He salido de mi sección crítica\n");
                getchar();
                memoria[ajeno] = 1;
                printf("He accionado el pulsador\n");
                continue;
            }
            else {
                printf("Puerta cerrada\n");
                continue;
            }
        }
    }
    if(shmdt(memoria) == -1) {
        printf("No se ha podido desenlazar el segmento: %s\n", strerror(errno));
    }
    return 0;
}
