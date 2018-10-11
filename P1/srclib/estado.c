

#include "estado.h"

struct _Estado{
	char* nombre; 	//Nombre del estado
	int tipo;		//Tipo de estado
	int** transiciones; //Matriz de transiciones
	int num_estados;
	int num_simbolos;
};

/*
Función para crear un nuevo estado, reserva memoria para 
el nombre.
*/
Estado* estadoNuevo(char* nombre, int tipo, int num_estados, int num_simbolos){
	if(!nombre) return NULL;

	Estado* est = malloc(sizeof(Estado));
	if(!est) return NULL;

	est->nombre = strdup(nombre); //Crea copia, reservando memoria
	if(!est->nombre){free(est); return NULL;}

	est->tipo = tipo;

	est->transiciones = (int**)malloc(sizeof(int*)*num_simbolos);
	if(!est->nombre){free(est->nombre);free(est); return NULL;}

	for(int i = 0; i < num_simbolos; i++){
		est->transiciones[i] = (int*)calloc(num_estados,sizeof(int));
	}

	return est;
}

/*
Inserta la transición
Dada por el simbolo i-ésimo y con destino el estado j-ésimo
*/
void estadoInsertaTransicion(Estado* p_s, int i, int j){
	p_s->transiciones[i][j] = 1;
}

/*
Elimina el estado p_s.
*/
void estadoElimina(Estado* p_s){
	if(!p_s) return;
	if(p_s->nombre) free(p_s->nombre);
	for(int i = 0; i < p_s->num_simbolos; i++){
		free(p_s->transiciones[i]);
	}
	free(p_s->transiciones);
	free(p_s);
	return;
}

/*
Imprime un estado en el fichero indicado.
*/
void estadoImprime(FILE* fd, Estado* p_s){
	int j = 0;
	if(!p_s || !fd) return;

	if(p_s->tipo == INICIAL || p_s->tipo == INICIAL_Y_FINAL) printf("->");
	printf("%s", p_s->nombre);
	if(p_s->tipo == FINAL || p_s->tipo == INICIAL_Y_FINAL) printf("*");
	printf(" ");
	// for(int i = 0; i < num_simbolos; i++){
	// 	for(j = 0; j < num_estados; j++){
	// 		printf("%d ", p_s->transiciones[i][j]);
	// 	}
	// 	printf("\n");
	// }
	return;
}

/*
Devuelve 1 si p_s tiene dicho nombre.
*/
int estadoEs(Estado* p_s, char* nombre){
	if(!p_s || !nombre) return 0;
	return (!strcmp(nombre, p_s->nombre));
}

/*
Devuelve el nombre de p_s.
*/
char* estadoNombre(Estado* p_s){
	if(!p_s) return NULL;
	return p_s->nombre;
}

/*
Devuelve el tipo de p_s.
*/
int estadoTipo(Estado* p_s){
	if(!p_s) return -1;
	return p_s->tipo;
}

/*
Devuelve la transición en la posición i
*/
int* getTransicion(Estado* p_s, int i){
	return p_s->transiciones[i];
}