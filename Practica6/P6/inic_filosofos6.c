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

int cola[6];

int main() {
    int estado;
    struct filosofo filosofo;

   

  
    for (int i = 1; i<6; i++) {
    cola[i] = msgget(IPC_PRIVATE, 0777 | IPC_CREAT); //Pruebe con 0777 | IPC_CREAT | IPC_EXCL
    if (cola[i] == -1) {
    perror("msgget");
    exit(EXIT_FAILURE);
    }
    
    //Llenar el filosofo a enviar
    filosofo.tipo = i; //tipo de filosofo
    sprintf(filosofo.texto, "filosofo de prueba"); //contenido del filosofo

    //Enviar el filosofo a la cola
    estado = msgsnd(cola[i], &filosofo, sizeof(struct filosofo) - sizeof(long), 0);
    if (estado == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("filosofo enviado: %s\n", filosofo.texto);
    }

    //Comprobar que el filosofo se ha almacenado en la cola correctamente
    system("ipcs -q");
}
