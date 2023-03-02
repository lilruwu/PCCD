#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>

int main(int argc, char *argv[]){
	int turno = atoi(argv[1]);

	key_t key = ftok("/home/Practica3/keys.txt", 'L');
	
	int id = shmget(key, sizeof(int), (0666|IPC_CREAT));
	if(id == -1){
		printf("No se ha podido crear el segmento\n");
		return -1;
	}

	printf("Id de la zona de memoria: %d\n", id);
	int *memoria = shmat(id, (void *)0, 0);
	if(memoria == (int *)(-1)){
		printf("No se ha podido enlazar el segmento\n");
		return -1;
	}else{
		*memoria = -1;
		while(1){
			printf("Caminando por mi habitación.\n");
			getchar();
			printf("Intentando entrar en mi sección crítica...\n");
			if(turno == *memoria || *memoria == -1){
				printf("Dentro de mi sección crítica\n");
				getchar();
				printf("He salido de mi sección crítica\n");
				getchar();
				*memoria = -1;
				printf("He accionado el pulsador\n");
				continue;
			}else{
				printf("Puerta cerrada\n");
				continue;
			}
			*memoria = turno;
			printf("He accionado el pulsador\n");
		}
	}
	if(shmdt(memoria) == -1){
		printf("No se ha podido desenlazar el segmento\n");
	}
	return 0;
}
