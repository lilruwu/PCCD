#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



struct mensaje {
    long tipo;
    char texto[50];
};

int main(int argc, char const *argv[]) {
    int cola, estado;
    struct mensaje mensaje;
    
    cola = atoi(argv[1]);
    //Obtener identificador de la cola de mensajes creada por el proceso p1
    
    estado = msgrcv(cola, &mensaje, sizeof(struct mensaje) - sizeof(long),0,0); //con el ultimo 0 es modo bloqueante si en vez del 0 pones reciebe no wait no se queda suspendido si no hay mensaje
    if (estado == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    printf("Mensaje recibido: %s\n", mensaje.texto);

    system("ipcs -q");
    return 0;
}