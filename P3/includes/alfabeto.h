#ifndef _ALFABETO_H
#define _ALFABETO_H

typedef struct _Alfabeto Alfabeto;

/*
Funcion para crear un alfabeto nuevo reservando memoria
*/
Alfabeto * alfabetoNuevo(char * nombre, int tamano);

/*
Devuelve el indice del simbolo s
*/
int getIndice(Alfabeto* alf, char* s);

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

/*
Devuelve el símbolo en la posición i
*/
char* alfabetoGetSimbolo(Alfabeto* alf, int i);

/*Devuelve todos los simbolos del alfabeto*/
char** alfabetoGetTodosSimbolos(Alfabeto *alf);

#endif