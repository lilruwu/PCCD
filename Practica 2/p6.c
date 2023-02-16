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
        case -1:         perror ("fork");
                        exit(-1);

        case 0:         execl("/bin/echo", "echo", "-n", "Carvajal ", NULL); //-n evita imprimir una nueva linea despues de cada nombre
                        exit(0);
    }

    switch(pid[1]=fork()){
        case -1:         perror ("fork");
                        exit(-1);

        case 0:         execl("/bin/echo", "echo", "-n", "Ramos ", NULL);
                        exit(0);
    }

    switch(pid[2]=fork()){
        case -1:         perror ("fork");
                        exit(-1);

        case 0:         execl("/bin/echo", "echo", "-n", "Jordi Alba ", NULL);
                        exit(0);
    }

    
    printf("Piqué ");
    

    switch(auxThiago=fork()){
        case -1:     perror ("fork");
                    exit(-1);
        case 0:     execl("/bin/echo", "echo", "-n", "Thiago ", NULL);
                    execl("/bin/echo", "echo", "-n", "Silva ", NULL);
                    exit(0);
    }

    while(contador != 3){
        finHijo=wait(NULL);

        if(finHijo==pid[0] || finHijo==pid[1] || finHijo==pid[2]) contador++;
        else if(finHijo==auxThiago) fin=1;

    }

    execl("/bin/echo", "echo", "-n", "Busquets ", NULL);
    execl("/bin/echo", "echo", "-n", "Isco ", NULL);
    execl("/bin/echo", "echo", "-n", "Aspas ", NULL);

    if(fin==0) wait(NULL);

    execl("/bin/echo", "echo", "Morata\n", NULL);

    return 0;
}
