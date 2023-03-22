#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int cola;
int cola2;

struct filosofo {
    long tipo;
    char texto[50];
};

struct ticket {
    long tipo;
    char texto[50];
};

void coger_tenedor(int i) {

    if (i>=6){
        i = 1;
    }
    printf("Intento coger el tenedor %i\n",i);
    struct filosofo filosofo;
    int estado;
    
    estado = msgrcv(cola, &filosofo, sizeof(struct filosofo) - sizeof(long),i,0); //con el ultimo 0 es modo bloqueante si en vez del 0 pones reciebe no wait no se queda suspendido si no hay filosofo
    if (estado == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    printf("Ha cogido el tenedor %i\n",i);
}
void soltar_tenedor(int i){
    if (i>=6){
        i = 1;
    }
    struct filosofo filosofo;
    int estado;
    filosofo.tipo = i; //tipo de filosofo
    sprintf(filosofo.texto, "filosofo de prueba");
    estado = msgsnd(cola, &filosofo, sizeof(struct filosofo) - sizeof(long), 0);
    if (estado == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("Ha soltado el tenedor %i\n",i);
}

void coger_ticket(int i) {
    printf("Intento coger el ticket %i\n",i);
    struct ticket ticket;
    int estado;
    estado = msgrcv(cola2, &ticket, sizeof(struct ticket) - sizeof(long),10,0); //con el ultimo 0 es modo bloqueante si en vez del 0 pones reciebe no wait no se queda suspendido si no hay filosofo
    if (estado == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    printf("Ha cogido el ticket %i\n",i);
}
void dejar_ticket(int i){
    
    struct ticket ticket;
    int estado;

    ticket.tipo = 10; //tipo de filosofo
    sprintf(ticket.texto, "Ticket de prueba");
    estado = msgsnd(cola2, &ticket, sizeof(struct ticket) - sizeof(long), 0);
    if (estado == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("Ha soltado el ticket %i\n",i);
}



int main(int argc, char **argv) {
    int i = atoi(argv[1]);
    cola = atoi(argv[2]);
    cola2 = atoi(argv[3]);
    struct filosofo filosofo;
    coger_ticket(i);
    getchar();
    coger_tenedor(i);
    getchar();
    coger_tenedor(i+1);
    getchar();
    system("ipcs -q");
    
    soltar_tenedor(i);
    getchar();
    soltar_tenedor(i+1);
    getchar();
    dejar_ticket(i);
    system("ipcs -q");

    return 0;
}
