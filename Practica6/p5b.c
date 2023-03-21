#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

 struct filosofo {
    long tipo;
    char texto[50];
};

void coger_tenedor(int i) {
    printf("Ha cogido el tenedor %i\n",i);
}

 int main(int argc, char **argv) {
         int i = atoi(argv[1]);
         struct filosofo filosofo;
         struct msqid_ds info_cola;

         int cola, estado,a;
         cola = 22;


if((a = msgctl(cola, IPC_RMID, &info_cola)) == -1) {
                 perror("msgctl");
                 exit(1);
         }
if (a>=5) {
    printf("COLA LLENA");
} else {
    //Obtener identificador de la cola de filosofos creada por el proceso p1
    
    coger_tenedor(i);
    coger_tenedor(i-1);

    //Llenar el filosofo a enviar
    filosofo.tipo = 1; //tipo de filosofo
    sprintf(filosofo.texto, "%i\n",i); //contenido del filosofo

    //Enviar el filosofo a la cola
    estado = msgsnd(cola, &filosofo, sizeof(struct filosofo) - sizeof(long), 0);
    if (estado == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("Filosofo: %s\n", filosofo.texto);

    system("ipcs -q");

    return 0;
 }
 }