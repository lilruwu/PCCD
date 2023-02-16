#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){

    int pid[3];
    int contador = 0;
    int auxThiago = 0;
    int finHijo = 0;
    int fin = 0;

    printf("De Gea ");

    switch(pid[0]=fork()){
        case -1:
            perror("fork");
            exit(-1);
        case 0:
            execl("/bin/echo", "echo", "-n", "Carvajal ", NULL);
            perror("execl");
            exit(-1);
    }

    switch(pid[1]=fork()){
        case -1:
            perror("fork");
            exit(-1);
        case 0:
            execl("/bin/echo", "echo", "-n", "Ramos ", NULL);
            perror("execl");
            exit(-1);
    }

    switch(pid[2]=fork()){
        case -1:
            perror("fork");
            exit(-1);
        case 0:
            execl("/bin/echo", "echo", "-n", "Jordi Alba ", NULL);
            perror("execl");
            exit(-1);
    }

    printf("Piqué ");

    switch(auxThiago=fork()){
        case -1:
            perror("fork");
            exit(-1);
        case 0:
            execl("/bin/echo", "echo", "-n", "Thiago Silva ", NULL);
            perror("execl");
            exit(-1);
    }

    while(contador != 3){
        finHijo=wait(NULL);

        if(finHijo==pid[0] || finHijo==pid[1] || finHijo==pid[2]) contador++;
        else if(finHijo==auxThiago) fin=1;
    }

    printf("Busquets ");
    printf("Isco ");
    printf("Aspas ");

    if(fin==0) wait(NULL);

    printf("Morata\n");

    return 0;
}
