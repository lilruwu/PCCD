#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CLAVE 3

struct filosofo {
    long tipo;
    char texto[50];
};

int main(int argc, char const *argv[]) {
    int cola, estado;
    struct filosofo filosofo;

    //Obtener identificador de la cola de filosofos creada por el proceso p1
    cola = atoi(argv[1]);
    if (cola == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i<6; i++) {

    
    //Llenar el filosofo a enviar
    filosofo.tipo = i; //tipo de filosofo
    sprintf(filosofo.texto, "filosofo de prueba"); //contenido del filosofo

    //Enviar el filosofo a la cola
    estado = msgsnd(cola, &filosofo, sizeof(struct filosofo) - sizeof(long), 0);
    if (estado == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("filosofo enviado: %s\n", filosofo.texto);
    }

    //Comprobar que el filosofo se ha almacenado en la cola correctamente
    system("ipcs -q");

    return 0;
}