#ifndef _ALFABETO_H
#define _ALFABETO_H

typedef struct _Alfabeto{
	char *nombre;
	int tamano;
	char **simbolos;

} Alfabeto;

/*
Funcion para crear un alfabeto nuevo reservando memoria
*/
Alfabeto * alfabetoNuevo(char * nombre, int tamano);

/*
Funcion para eliminar un elfabeto y liberar toda su memoria
*/
void alfabetoElimina(Alfabeto * p_alfabeto);

/*
Funcion para insertar un simbolo en la coleccion del alfabeto
*/
Alfabeto * alfabetoInsertaSimbolo(Alfabeto * p_alfabeto, char * simbolo);

/*
Funcion para imprimir un alfabeto por el FILE argumento
*/
void alfabetoImprime(FILE * fd, Alfabeto * p_alf);

#endif