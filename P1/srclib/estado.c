

#include "estado.h"

struct _Estado{
	char* nombre; 	//Nombre del estado
	int tipo;		//Tipo de estado
};

/*
Función para crear un nuevo estado, reserva memoria para 
el nombre.
*/
Estado* estadoNuevo(char* nombre, int tipo){
	if(!nombre) return NULL;

	Estado* est = malloc(sizeof(Estado));
	if(!est) return NULL;

	est->nombre = strdup(nombre); //Crea copia, reservando memoria
	if(!est->nombre){free(est); return NULL;}

	est->tipo = tipo;

	return est;
}

/*
Elimina el estado p_s.
*/
void estadoElimina(Estado* p_s){
	if(!p_s) return;
	if(p_s->nombre) free(p_s->nombre);
	free(p_s);
	return;
}

/*
Imprime un estado en el fichero indicado.
*/
void estadoImprime(FILE* fd, Estado* p_s){
	if(!p_s || !fd) return;

	if(p_s->tipo == INICIAL || p_s->tipo == INICIAL_Y_FINAL) printf("->");
	printf("%s", p_s->nombre);
	if(p_s->tipo == FINAL || p_s->tipo == INICIAL_Y_FINAL) printf("*");
	printf(" ");
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