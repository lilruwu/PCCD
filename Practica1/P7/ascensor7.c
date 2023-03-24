#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int piso = 0;
int max_pisos;
int T_PISO = 3;
int fin = 0;
int j = 0;
int PIDsensor[40] = {};

void signalHandler(int signum) {
    if (signum == SIGUSR1) { //subir piso
        if (piso != max_pisos) {
            piso++;
            printf("Subiendo piso...\n");
            cambiarPiso();
            printf("Estas en el piso: %d\n", piso);
            kill(PIDsensor[piso],1);
        } else {
            printf("Ya estas en el ultimo piso!\n");
        }

    } else if (signum == SIGUSR2) { //bajar piso
        if (piso != 0) {
            piso--;
            printf("Bajando piso...\n");
            cambiarPiso();
            printf("Estas en el piso: %d\n", piso);
            kill(PIDsensor[piso],1);
        } else {
            printf("Ya estas en el piso cero!\n");
        }
    } else if (signum == SIGQUIT) { //salir del ascensor
        fin = 1;
        printf("Has salido del ascensor!\n");
        exit(0);
    }
}

void cambiarPiso() {
    sleep(T_PISO);
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("Se requiere un argumento: el numero maximo de pisos.\n");
        return 1;
    }
    max_pisos = atoi(argv[1]);

    for (j=0; j<=max_pisos; j++){

		printf("Por favor, introduzca el PID del sensor del piso %i: ", j);
		scanf("%i", &PIDsensor[j]);
	}

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
