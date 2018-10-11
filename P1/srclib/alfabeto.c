#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alfabeto.h"

struct _Alfabeto{
	char *nombre;
	int tamano;
	char **simbolos;
};

/*
Devuelve el símbolo en la posición i
*/
char* alfabetoGetSimbolo(Alfabeto* alf, int i){
	return alf->simbolos[i];
}

/*
Devuelve el indice del simbolo s
*/
int getIndice(Alfabeto* alf, char* s){
	for(int i = 0; i < alf->tamano; i++){
		if(strcmp(alf->simbolos[i], s) == 0)
			return i;
	}
	return -1;
}

/*
Funcion para crear un alfabeto nuevo reservando memoria
*/
Alfabeto * alfabetoNuevo(char * nombre, int tamano){
	char *nuevo_nombre = NULL;
	Alfabeto *alfabeto = NULL;

	/*Reserva de memoria*/
	alfabeto = (Alfabeto *)malloc(sizeof(Alfabeto));
	if(!alfabeto)
		exit(1);

	nuevo_nombre = strdup(nombre); /*Ya copia nombre a nuevo_nombre*/
	if(!nuevo_nombre){
		free(alfabeto);
		exit(1);
	}

	alfabeto->simbolos = (char **)malloc(tamano * sizeof(char*));
	if(!alfabeto->simbolos){
		free(nuevo_nombre);
		free(alfabeto);
		exit(1);
	}

	/*Rellenando la estructura*/
	alfabeto->tamano = 0;

	alfabeto->nombre = nuevo_nombre;

	return alfabeto;

}

void simbolosElimina(char **simbolos, int tamano){
	int i;

	for(i = 0; i < tamano; i++){
		free(simbolos[i]);
	}

	free(simbolos);
}

/*
Funcion para eliminar un elfabeto y liberar toda su memoria
*/
void alfabetoElimina(Alfabeto * p_alfabeto){
	if(p_alfabeto){
		
		if(p_alfabeto->nombre)
			free(p_alfabeto->nombre);

		if(p_alfabeto->simbolos)
			simbolosElimina(p_alfabeto->simbolos, p_alfabeto->tamano);

		free(p_alfabeto);

	}
}

/*
Funcion para insertar una copia de un simbolo en la coleccion del alfabeto
*/
Alfabeto * alfabetoInsertaSimbolo(Alfabeto * p_alfabeto, char * simbolo){
	char *nuevo_simbolo = NULL;

	if(!p_alfabeto || !simbolo)
		exit(1);

	/*Creamos una copia del simbolo a insertar*/
	nuevo_simbolo = strdup(simbolo);
	if(!nuevo_simbolo)
		exit(1);


	(p_alfabeto->simbolos)[p_alfabeto->tamano] = nuevo_simbolo;

	p_alfabeto->tamano ++;

	return p_alfabeto;
}

/*
Funcion para imprimir un alfabeto por el FILE argumento
*/
void alfabetoImprime(FILE * fd, Alfabeto * p_alf){
	int i;

	if(!fd || !p_alf)
		exit(1);

	fprintf(fd, "%s={", p_alf->nombre);
	for(i = 0; i < p_alf->tamano; i++){

		fprintf(fd, " %s ", (p_alf->simbolos)[i]);
	}
	fprintf(fd, "}\n");

	return;
}