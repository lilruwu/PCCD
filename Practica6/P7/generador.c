#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

struct producto {
    long tipo;
    char texto[50];
    int producto1;
    int producto2;
};

int cola;
int contador = 0;

int main(int argc, char **argv){
    
    

    cola = msgget(IPC_PRIVATE, 0777 | IPC_CREAT); //Pruebe con 0777 | IPC_CREAT | IPC_EXCL
    if (cola == -1) {
    perror("msgget");
    exit(EXIT_FAILURE);
    }
    
    while(1){
    int estado;
    struct producto producto;
    
    srand(contador);
    producto.producto1=rand()%3 + 1;
    producto.producto2=rand()%3 + 1;
    contador++;
    //Obtener identificador de la cola de fumadors creada por el proceso p1
    


    
    //Llenar el fumador a enviar
    producto.tipo = 1; //tipo de fumador
    sprintf(producto.texto, "producto de prueba"); //contenido del fumador

    //Enviar el fumador a la cola
    sleep(1);
    estado = msgsnd(cola, &producto, sizeof(struct producto) - sizeof(long), 0);
    if (estado == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    printf("Productos generados\n");
    

    //Comprobar que el fumador se ha almacenado en la cola correctamente
    system("ipcs -q");
    printf("Productor esperando a que fumen\n");
    estado = msgrcv(cola, &producto, sizeof(struct producto) - sizeof(long),2,0); //con el ultimo 0 es modo bloqueante si en vez del 0 pones reciebe no wait no se queda suspendido si no hay fumador
    if (estado == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    system("ipcs -q");
    }
    return 0;
    }
