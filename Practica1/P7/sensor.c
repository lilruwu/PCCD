#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int piso;
pid_t pid;
int fin = 0;

void signalHandler(int signum) {
    if (signum == SIGHUP) { //subir piso
        printf("El ascensor ha llegado al piso %d\n", piso);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Se requiere un argumento: piso del ascensor.\n");
        return 1;
    }
    piso = atoi(argv[1]);
    pid = getpid();
    printf("El pid del proceso es: %d\n", pid);

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = signalHandler; //le asignas el handler a la estructura sigaction
    sigfillset(&action.sa_mask);

    sigaction(SIGHUP, &action, NULL);

    while (!fin) {
        pause();
    }
    return 0;
}
