#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  int i;
  int hijos_finalizados = 0;
  pid_t pid[3];

  for (i = 0; i < 3; i++) {
    pid[i] = fork();

    if (pid[i] == -1) {
      perror("Error al crear el proceso hijo");
      return 1;
    } else if (pid[i] == 0) {
      printf("Soy el hijo %d, durmiendo por 5 segundos...\n", i + 1);
      sleep(5);
      return 0;
    }
  }

  // Proceso padre
  while (hijos_finalizados < 3) {
    pid_t hijo_finalizado = wait(NULL);
    hijos_finalizados++;
    printf("El hijo con ID %d finalizó su ejecución\n", hijo_finalizado);
    pause();
  }

  printf("Todos los hijos han finalizado su ejecución\n");
  return 0;
}
