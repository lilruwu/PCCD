#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int valor;
int fin = 0;

int main(int argc, char *argv[]){
    while(!fin){
    if (argc != 2) {
        printf("Uso: %s PID\n", argv[0]);
        return 1;
    }

    pid_t pid = (pid_t) atoi(argv[1]);

    printf("ESCOJA UNA ACCION:\n");
    printf("1 - SUBIR\n");
    printf("2 - BAJAR\n");
    printf("3 - SALIR\n");

    scanf("%i", &valor);

    switch(valor) {
        case 1:
            if (kill(pid, SIGUSR1) == -1) {
                perror("kill");
                return 1;
            }
            break;

        case 2:
            if (kill(pid, SIGUSR2) == -1) {
                perror("kill");
                return 1;
            }
            break;

        case 3:
            if (kill(pid, SIGQUIT) == -1) {
                fin = 1;
                perror("kill");
                return 1;
            }
            break;

        default:
            printf("Opcion no valida\n");
            break;
    }
    }
    return 0;
}
