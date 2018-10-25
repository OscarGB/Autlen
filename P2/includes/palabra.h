
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _PALABRA_H
#define _PALABRA_H

typedef struct _Palabra Palabra;

/*
Función para crear una nueva palabra.
*/
Palabra* palabraNueva();

/*
Elimina la palabra p_p.
*/
void palabraElimina(Palabra* p_p);

/*
Inserta una letra (ojo, es un char*, puede ser "alpha"
o una palabra "del lenguaje natural" entera).
*/
Palabra* palabraInsertaLetra(Palabra* p_p, char* letra);

/*
Imprime una palabra por el fichero indicado.
*/
void palabraImprime(FILE *fd, Palabra* p_p);

/*
Hace pop de la primera letra.
*/
char* palabraQuitaInicio(Palabra* p_p);

/*
Devuelve la letra en la posicion i
*/
char* palabraGetILetra(Palabra* p_p, int i);

/*
Devuelve el Tamaño de una palabra.
*/
int palabraTamano(Palabra* p_p);



#endif //_PALABRA_H