#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
int i;
int hijos_fin = 0;

for (i=0;i<3;i++){
    pid_t pid[3];
    pid[i] = fork();

    if (pid[i]==-1) {
        perror("Error al crear el proceso hijo");
        return 1;
    } else if (pid[i]==0) {
        printf("Soy el hijo, voy a mimi\n");
        sleep(5);
        printf("Ya he despertado!\n");
    }
}

  while (hijos_finalizados < 3) {
    pid_t hijo_finalizado = wait(NULL);
    hijos_finalizados++;
    printf("El hijo con ID %d finalizó su ejecución\n", hijo_finalizado);
    pause();
  }
    printf("Todos los hijos han finalizado su ejecución\n");
    return 0;
}
