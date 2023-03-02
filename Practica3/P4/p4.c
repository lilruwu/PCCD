#include <stdio.h>
#include <stdlib.h>

int main(){

	while (1){
		printf("Caminando por la habitación...\n");
		getchar();
		printf("Intentando entrar en mi sección crítica.\n");

		//SECCION CRITICA
		printf("Dentro de mi sección crítica.\n");
		getchar();
		//FIN SECCION CRITICA

		printf("He salido de mi sección crítica.\n");
	}
	return 0;
}