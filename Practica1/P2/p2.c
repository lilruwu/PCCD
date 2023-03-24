#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
    if (argc != 3) {
        printf("Uso: %s <pid> <signal>\n", argv[0]);
        return 1;
    }

    pid_t pid = (pid_t) atoi(argv[1]);
    int sig = atoi(argv[2]);

    if (kill(pid, sig) == -1) {
        perror("kill");
        return 1;
    }

    return 0;
}
