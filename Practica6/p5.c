#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int cola = 3;

struct filosofo {
    long tipo;
    char texto[50];
};

void coger_tenedor(int i) {
    printf("Ha cogido el tenedor %i\n",i);
    struct filosofo filosofo;
    int estado;
    estado = msgrcv(cola, &filosofo, sizeof(struct filosofo) - sizeof(long),i,0); //con el ultimo 0 es modo bloqueante si en vez del 0 pones reciebe no wait no se queda suspendido si no hay filosofo
    if (estado == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
}
void soltar_tenedor(int i){
    printf("Ha soltado el tenedor %i\n",i);
    struct filosofo filosofo;
    int estado;
    filosofo.tipo = i; //tipo de filosofo
    sprintf(filosofo.texto, "filosofo de prueba");
    estado = msgsnd(cola, &filosofo, sizeof(struct filosofo) - sizeof(long), 0);
    if (estado == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    int i = atoi(argv[1]);
    struct filosofo filosofo;

    // Crear la cola de filosofos
    /*int estado_creacion = 0;
    int permisos = IPC_CREAT | 0666;
    estado_creacion = msgget(cola, permisos);
    if (estado_creacion == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }*/

    coger_tenedor(i);
    coger_tenedor(i+1);
    system("ipcs -q");
    
    soltar_tenedor(i);
    soltar_tenedor(i+1);

    //Llenar el filosofo a enviar

    //Enviar el filosofo a la cola
 
    

    system("ipcs -q");

    return 0;
}
