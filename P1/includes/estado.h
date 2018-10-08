
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _ESTADO_H
#define _ESTADO_H

#define INICIAL 0
#define FINAL 1
#define INICIAL_Y_FINAL 2
#define NORMAL 3

typedef struct _Estado Estado;

/*
Función para crear un nuevo estado, reserva memoria para 
el nombre.
*/
Estado* estadoNuevo(char* nombre, int tipo);

/*
Elimina el estado est.
*/
void estadoElimina(Estado* p_s);

/*
Imprime un estado en el fichero indicado.
*/
void estadoImprime(FILE* fd, Estado* p_s);

/*
Devuelve 1 si p_s tiene dicho nombre.
*/
int estadoEs(Estado* p_s, char* nombre);

/*
Devuelve el nombre de p_s.
*/
char* estadoNombre(Estado* p_s);

/*
Devuelve el tipo de p_s.
*/
int estadoTipo(Estado* p_s);

#endif //_ESTADO_H