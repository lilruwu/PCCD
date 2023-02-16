#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("Soy el hijo %d, imprimiendo la lista de parámetros...\n", atoi(argv[0])+1);
    execl("/bin/sh", "sh", "-c", "echo $*", argv[1], argv[2], argv[3], (char *)0);
    perror("Error al invocar a execl");
    return 1;
}
