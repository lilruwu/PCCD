#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

struct ticket {
    long tipo;
    char texto[50];
};


int cola2;

int main(int argc, char **argv){
    int estado;
    struct ticket ticket;
    cola2 = atoi(argv[1]);
    //Obtener identificador de la cola de filosofos creada por el proceso p1
    
    for (int i = 1; i<5; i++) {

    
    //Llenar el filosofo a enviar
    ticket.tipo = 10; //tipo de filosofo
    sprintf(ticket.texto, "ticket de prueba"); //contenido del filosofo

    //Enviar el filosofo a la cola
    estado = msgsnd(cola2, &ticket, sizeof(struct ticket) - sizeof(long), 0);
    if (estado == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("filosofo enviado: %s\n", ticket.texto);
    }

    //Comprobar que el filosofo se ha almacenado en la cola correctamente
    system("ipcs -q");

}