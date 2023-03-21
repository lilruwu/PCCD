#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  int cola;

  cola = msgget(IPC_PRIVATE, 0777 | IPC_CREAT); //Pruebe con 0777 | IPC_CREAT | IPC_EXCL
  if (cola == -1) {
    perror("msgget");
    exit(EXIT_FAILURE);
  }
  printf("Colas creadas: %d\n", cola);
  return 0;
}
