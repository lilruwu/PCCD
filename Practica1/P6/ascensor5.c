#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int piso;
int max_pisos;
int T_PISO = 3;
int fin = 0;

void signalHandler(int signum) {
    if (signum == SIGUSR1) { //subir piso
        if (piso!=max_pisos) {
            piso++;
            printf("Subiendo piso...\n");
            cambiarPiso();
            printf("Estás en el piso: %d\n", piso);
        } else {
            printf("Ya estás en el último piso!\n");
        }

    } else if (signum == SIGUSR2) { //bajar piso
        if (piso!=0) {
            piso--;
            printf("Bajando piso...\n");
            cambiarPiso();
            printf("Estás en el piso: %d\n", piso);
        } else {
            printf("Ya estás en el piso cero!\n");
        }
    } else if (signum == SIGQUIT) { //salir del ascensor
        fin = 1;
        printf("Has salido del ascensor!\n");
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Se requiere un argumento: el número máximo de pisos.\n");
        return 1;
    }
    max_pisos = atoi(argv[1]);

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = signalHandler; //le asignas el handler a la estructura sigaction
    sigfillset(&action.sa_mask);

    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);

    while (!fin) {
        pause();
    }
    return 0;
}

void cambiarPiso() {

    sleep(T_PISO);
    return;
}
