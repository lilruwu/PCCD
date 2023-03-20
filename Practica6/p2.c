#include <sys/types.h>
 #include <sys/ipc.h>
 #include <sys/msg.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <errno.h>

 int main(int argc, char **argv) {
         int id = atoi(argv[1]);
         int resultado;

         if((resultado = msgctl(id, IPC_RMID, 0)) == -1) {
                 perror("msgctl");
                 exit(1);
         }
         printf("Cola eliminada: %d\n", resultado);

         exit(0);
 }