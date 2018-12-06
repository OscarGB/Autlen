#include <stdio.h>
#include <stdlib.h>

#ifndef _AFND_H
#define _AFND_H

typedef struct _AFND AFND;

/*
Crea un nuevo AFND
*/
AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos);

/*
Elimina un AFND, liberando toda la memoria
*/
void AFNDElimina(AFND * p_afnd);

/*
Imprime un AFND
*/
void AFNDImprime(FILE * fd, AFND* p_afnd);

/*
Inserta un simbolo en el AFND
*/
AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo);


/*
Inserta un estado en el AFND
*/
AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo);

/*
Inserta una transicion al AFND
*/
AFND * AFNDInsertaTransicion(AFND * p_afnd, 
                             char * nombre_estado_i, 
                             char * nombre_simbolo_entrada, 
                             char * nombre_estado_f );

/*
Inserta una letra en el AFND
*/
AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra);

/*
Imprime el conjunto de estados actuales del AFND
*/
void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd);

/*
Imprime la cadena actual del AFND
*/
void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd);

/*
Inicializa el estado de un AFND
*/
AFND * AFNDInicializaEstado (AFND * p_afnd);

/*
Procesa la entrada segun el AFND
*/
void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd);

/*
Hace que el AFND transite
*/
void AFNDTransita(AFND * p_afnd);

/*
No hace nada, pero nos lo piden
*/
AFND * AFNDInicializaCadenaActual(AFND * p_afnd_l);

/*
Calcula la matriz de transiciones lambda
*/
void AFNDCierraLTransicion(AFND * p_afnd);

/*
Inserta una transicion lambda al AFND
*/
AFND * AFNDInsertaLTransicion(AFND * p_afnd, 
                             char * nombre_estado_i,  
                             char * nombre_estado_f );

/*
Crea un nuevo automata con un solo simbolo
*/
AFND * AFND1ODeSimbolo(char * simbolo);

/*
Crea un nuevo automata con un lambda
*/
AFND * AFND1ODeLambda();

/*
Crea un nuevo automata vacio
*/
AFND * AFND1ODeVacio();

/*
Crea un afnd a partir de dos dados, uniendolos
*/
AFND * AFND1OUne(AFND * p_afnd1O_1, AFND * p_afnd1O_2);

/*
Crea un afnd a partir de dos dados, concatenandolos
*/
AFND * AFND1OConcatena(AFND * p_afnd1O_1, AFND * p_afnd1O_2);

/*
Crea un afnd a partir de uno dado, aplicando la operación asterisco
*/
AFND * AFND1OEstrella(AFND * p_afnd1O_1);

/*
Transforma cualquier AFND a 1O
*/
AFND * AFNDAAFND1O(AFND * p_afnd1);


/*
Imprime el diagrama en formato DOT
*/
void AFNDADot(FILE * fd, AFND * p_afnd);




#endif