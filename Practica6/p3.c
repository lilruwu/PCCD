#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CLAVE 3

struct mensaje {
    long tipo;
    char texto[50];
};

int main(int argc, char const *argv[]) {
    int cola, estado;
    struct mensaje mensaje;

    //Obtener identificador de la cola de mensajes creada por el proceso p1
    cola = atoi(argv[1]);
    if (cola == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    //Llenar el mensaje a enviar
    mensaje.tipo = 1; //tipo de mensaje
    sprintf(mensaje.texto, "Mensaje de prueba"); //contenido del mensaje

    //Enviar el mensaje a la cola
    estado = msgsnd(cola, &mensaje, sizeof(struct mensaje) - sizeof(long), 0);
    if (estado == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("Mensaje enviado: %s\n", mensaje.texto);

    //Comprobar que el mensaje se ha almacenado en la cola correctamente
    system("ipcs -q");

    return 0;
}