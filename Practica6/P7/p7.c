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
int tipo_fumador;



void coger_producto(int i) {
   
    printf("Intento coger un producto\n");
    struct producto producto;
    int estado;

    
    
    estado = msgrcv(cola, &producto, sizeof(struct producto) - sizeof(long),0,0); //con el ultimo 0 es modo bloqueante si en vez del 0 pones reciebe no wait no se queda suspendido si no hay fumador
    if (estado == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    switch(i){
        case 1: //tabaco
        if ((producto.producto1==2)||(producto.producto2==2)&&(producto.producto1==3)||(producto.producto2==3)){
            printf("El tipo con tabaco esta fumando\n");
            printf("No tiene papel y fósforo, dejo los productos\n");
            producto.tipo=2;
            estado = msgsnd(cola, &producto, sizeof(struct producto) - sizeof(long), 0);
            if (estado == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
        printf("El tipo con tabaco ha fumado\n");
        } else {
            printf("No tiene papel y fósforo, dejo los productos\n");
            estado = msgsnd(cola, &producto, sizeof(struct producto) - sizeof(long), 0);
            if (estado == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
        }
        break;
        case 2: //papel
        if ((producto.producto1==1)||(producto.producto2==1)&&(producto.producto1==3)||(producto.producto2==3)){
            printf("El tipo con papel esta fumando\n");
            producto.tipo=2;
            estado = msgsnd(cola, &producto, sizeof(struct producto) - sizeof(long), 0);
            if (estado == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
        printf("El tipo con papel ha fumado\n");
        } else {
            printf("No tiene tabaco y fósforo, dejo los productos\n");
            estado = msgsnd(cola, &producto, sizeof(struct producto) - sizeof(long), 0);
            if (estado == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
        }
        break;
        case 3: //fosforo
        if ((producto.producto1==1)||(producto.producto2==1)&&(producto.producto1==2)||(producto.producto2==2)){
            printf("El tipo con fósforo esta fumando\n");
            producto.tipo=2;
            estado = msgsnd(cola, &producto, sizeof(struct producto) - sizeof(long), 0);
            if (estado == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
        printf("El tipo con fósforo ha fumado\n");
        } else {
            printf("No tiene papel y tabaco, dejo los productos\n");
            estado = msgsnd(cola, &producto, sizeof(struct producto) - sizeof(long), 0);
            if (estado == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
        }
        break;
        default:
        printf("Ese pive no fuma");
    }
    
    system("ipcs -q");
}



int main(int argc, char **argv){
    int i;
    cola = atoi(argv[1]);
    i = atoi(argv[2]);
    while(1) {
    coger_producto(i);
    sleep(2);
    }
    
    
    return 0;
}