#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int sigusr1Count = 0;
int sigusr2Count = 0;

void signalHandler(int signum) {
    if (signum == SIGUSR1) {
        sigusr1Count++;
        printf("Señal SIGUSR1 recibida. Contador: %d\n", sigusr1Count);
    }
    else if (signum == SIGUSR2) {
        sigusr2Count++;
        printf("Señal SIGUSR2 recibida. Contador: %d\n", sigusr2Count);
    }
    else if (signum == SIGTERM) {
        printf("Señales SIGUSR1 recibidas: %d\n", sigusr1Count);
        printf("Señales SIGUSR2 recibidas: %d\n", sigusr2Count);
        exit(0);
    }
}

int main() {
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = signalHandler;
    action.sa_flags = SA_RESTART;

    if (sigaction(SIGUSR1, &action, NULL) == -1) {
        perror("sigaction");
    }
    if (sigaction(SIGUSR2, &action, NULL) == -1) {
        perror("sigaction");
    }
    if (sigaction(SIGTERM, &action, NULL) == -1) {
        perror("sigaction");
    }

    while (1) {
    pause();
    }
    return 0;
}
