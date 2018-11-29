#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"

/* ESTOS INCLUDES DEPENDEN DE LA IMPLEMENTACIÓN, TAL VEZ TÚ DISEÑES OTROS MÓDULOS */
#include "alfabeto.h"
#include "estado.h"
#include "palabra.h"


int main(int argc, char ** argv)
{
    
/* SE CREA UN AUTÓMATA FINITO PARA LA EXPRESION REGULAR “1” */
    AFND * p_afnd_l1 = AFND1ODeSimbolo("1");
     fprintf(stdout,"\n****************** AFND *********************\n");
    AFNDImprime(stdout,p_afnd_l1);
    fprintf(stdout,"\n*********************************************\n");

/* SE CREA UN AUTÓMATA FINITO PARA LA EXPRESION REGULAR “0” */
    AFND * p_afnd_l0 = AFND1ODeSimbolo("0");
     fprintf(stdout,"\n****************** AFND *********************\n");
    AFNDImprime(stdout,p_afnd_l0);
    fprintf(stdout,"\n*********************************************\n");

/* SE CREA UN AUTÓMATA FINITO PARA LA EXPRESION REGULAR “0”+“1” */
    AFND * p_afnd = AFNDAAFND1O(p_afnd_l0);
     fprintf(stdout,"\n****************** AFND *********************\n");
    AFNDImprime(stdout,p_afnd);
    fprintf(stdout,"\n*********************************************\n");


/* SE MUESTRA EL AFND DEFINIDO */
    fprintf(stdout,"\n****************** AFND *********************\n");
    AFNDImprime(stdout,p_afnd);
    fprintf(stdout,"\n*********************************************\n");

/* DEFINICIÓN DE LA CADENA DE ENTRADA [ 0 1 0 1 1 ] */
    p_afnd= AFNDInsertaLetra(p_afnd,"0");
    p_afnd= AFNDInsertaLetra(p_afnd,"1");
    p_afnd= AFNDInsertaLetra(p_afnd,"1");
    p_afnd= AFNDInsertaLetra(p_afnd,"0");

/* SE ESTABLECE COMO ESTADO ACTUAL DEL AUTÓMATA EL INICIAL */

    AFNDInicializaEstado (p_afnd);

/* SE MUESTRA LA CADENA ACTUAL */

    fprintf(stdout,"\n**************** PROCESA CADENA *************\n");
    AFNDImprimeCadenaActual(stdout,p_afnd);
    fprintf(stdout,"\n*********************************************\n");

/* SE PROCESA LA CADENA DE ENTRADA ACTUAL MOSTRANDO UNA TRAZA DEL FUNCIONAMIENTO DEL AUTOMATA: EN CADA PASO DE ANÁLISIS SE MUESTRA LA CADENA ACTUAL Y EL CONJUNTO DE ESTADOS EN LOS QUE SE ENCUENTRA EL AUTÓMATA */

    AFNDProcesaEntrada(stdout,p_afnd);


/* DEFINICIÓN DE LA CADENA DE ENTRADA [ 0 1 1 0 0 ] */
    p_afnd= AFNDInsertaLetra(p_afnd,"1");
    p_afnd= AFNDInsertaLetra(p_afnd,"1");
    p_afnd= AFNDInsertaLetra(p_afnd,"1");
    p_afnd= AFNDInsertaLetra(p_afnd,"0");

/* SE ESTABLECE COMO ESTADO ACTUAL DEL AUTÓMATA EL INICIAL */

    AFNDInicializaEstado (p_afnd);

/* SE MUESTRA LA CADENA ACTUAL */

    fprintf(stdout,"\n**************** PROCESA CADENA *************\n");
    AFNDImprimeCadenaActual(stdout,p_afnd);
    fprintf(stdout,"\n*********************************************\n");


/* SE PROCESA LA CADENA DE ENTRADA ACTUAL MOSTRANDO UNA TRAZA DEL FUNCIONAMIENTO DEL AUTOMATA: EN CADA PASO DE ANÁLISIS SE MUESTRA LA CADENA ACTUAL Y EL CONJUNTO DE ESTADOS EN LOS QUE SE ENCUENTRA EL AUTÓMATA */

    AFNDProcesaEntrada(stdout,p_afnd);

/* DEFINICIÓN DE LA CADENA DE ENTRADA [ 0 1 1 0 0 ] */
    p_afnd= AFNDInsertaLetra(p_afnd,"1");
    p_afnd= AFNDInsertaLetra(p_afnd,"1");
    p_afnd= AFNDInsertaLetra(p_afnd,"1");
    p_afnd= AFNDInsertaLetra(p_afnd,"0");
    p_afnd= AFNDInsertaLetra(p_afnd,"1");

/* SE ESTABLECE COMO ESTADO ACTUAL DEL AUTÓMATA EL INICIAL */

    AFNDInicializaEstado (p_afnd);

/* SE MUESTRA LA CADENA ACTUAL */

    fprintf(stdout,"\n**************** PROCESA CADENA *************\n");
    AFNDImprimeCadenaActual(stdout,p_afnd);
    fprintf(stdout,"\n*********************************************\n");


/* SE PROCESA LA CADENA DE ENTRADA ACTUAL MOSTRANDO UNA TRAZA DEL FUNCIONAMIENTO DEL AUTOMATA: EN CADA PASO DE ANÁLISIS SE MUESTRA LA CADENA ACTUAL Y EL CONJUNTO DE ESTADOS EN LOS QUE SE ENCUENTRA EL AUTÓMATA */

    AFNDProcesaEntrada(stdout,p_afnd);

// /* SE LIBERAN TODOS LOS RECURSOS ASOCIADOS CON EL AFND */
//     AFNDElimina(p_afnd);
//     AFNDElimina(p_afnd_l0);
//     AFNDElimina(p_afnd_l1);

    return 0;

}

