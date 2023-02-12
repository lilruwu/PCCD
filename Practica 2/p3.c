#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int i;
    int hijos_finalizados = 0;

    for (i = 0; i < 3; i++) {
        pid_t pid[3];
        pid[i] = fork();

        if (pid[i] == -1) {
            perror("Error al crear el proceso hijo");
            return 1;
        } else if (pid[i] == 0) {
            printf("Soy el hijo, voy a dormir\n");
            sleep(5);
            exit(i + 1);
        }
    }

    int status;
    pid_t hijo_finalizado;
    while (hijos_finalizados < 3) {
        hijo_finalizado = waitpid(-1, &status, WNOHANG);
        if (hijo_finalizado > 0) {
            hijos_finalizados++;
            printf("El hijo con ID %d finalizó su ejecución con código %d\n", hijo_finalizado, WEXITSTATUS(status));
        }
    }
    printf("Todos los hijos han finalizado su ejecución\n");
    return 0;
}
