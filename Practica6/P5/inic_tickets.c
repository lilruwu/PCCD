#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CLAVE 3

struct ticket {
    long tipo;
    char texto[50];
};

int main(int argc, char const *argv[]) {
    int cola, estado;
    struct ticket ticket;

    //Obtener identificador de la cola de filosofos creada por el proceso p1
    cola = atoi(argv[1]);
    if (cola == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i<5; i++) {

    
    //Llenar el filosofo a enviar
    ticket.tipo = 10; //tipo de filosofo
    sprintf(ticket.texto, "ticket de prueba"); //contenido del filosofo

    //Enviar el filosofo a la cola
    estado = msgsnd(cola, &ticket, sizeof(struct ticket) - sizeof(long), 0);
    if (estado == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("filosofo enviado: %s\n", ticket.texto);
    }

    //Comprobar que el filosofo se ha almacenado en la cola correctamente
    system("ipcs -q");

    return 0;
}