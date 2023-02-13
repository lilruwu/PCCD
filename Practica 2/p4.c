#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
int i;
int hijos_finalizados = 0;
pid_t pid[3];
int status;

for (i = 0; i < 3; i++) {
pid[i] = fork();

if (pid[i] == -1) {
  perror("Error al crear el proceso hijo");
  return 1;
} else if (pid[i] == 0) {
  printf("Soy el hijo %d, imprimiendo la lista de parámetros...\n", i + 1);
  execl("/bin/sh", "sh", "-c", "echo $*", argv[0], argv[1], argv[2], (char *)0);
  perror("Error al invocar a execl");
  return 1;
}

}

// Proceso padre
while (hijos_finalizados < 3) {
pid_t hijo_finalizado = wait(&status);
hijos_finalizados++;
printf("El hijo con ID %d finalizó su ejecución con código %d\n", hijo_finalizado, WEXITSTATUS(status));
}

printf("Todos los hijos han finalizado su ejecución\n");
return 0;
}
