#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_SIGNALS 64

bool signalsReceived[MAX_SIGNALS];

void signalHandler(int signum) { //se va a ejecutar siempre que se llame y asi modificas lo q se hace con cada senhal
    signalsReceived[signum] = true;
    printf("Señal %d recibida\n", signum);

    if (signum == SIGTERM) {
        printf("Vector de señales recibidas: [ ");
        int i;
        for (i = 1; i < MAX_SIGNALS; i++) {
            printf("%d ", signalsReceived[i]);
        }
        printf("]\n");
        exit(0);
    } else {
        signalsReceived[signum]=1;

    }
}

int main() {
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = signalHandler; //le asignas el handler a la estructura sigaction
    sigfillset(&action.sa_mask);
    int signum;
    for (signum = 1; signum < MAX_SIGNALS; signum++) {
        if (sigaction(signum, &action, NULL) == -1) {
            perror("sigaction");
        }
    }
    while (1) {
    pause();
    }
    return 0;
}
