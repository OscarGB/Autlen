
#include "afnd.h"

#include "palabra.h"
#include "alfabeto.h"
#include "estado.h"

struct _AFND {
    char * nombre;
    Alfabeto * alfabeto;
    int num_estados;
    int num_simbolos;
    int num_estados_actual;
    int * estados_actuales;
    Estado ** estados;
    Palabra * cadena_actual; 
};

/*
Crea un nuevo AFND
*/
AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos){
	AFND* afnd = NULL;
	afnd = (AFND*)malloc(sizeof(AFND));
	afnd->alfabeto = alfabetoNuevo("A", num_simbolos);
	afnd->nombre = strdup(nombre);
	afnd->num_simbolos = num_simbolos;
	afnd->num_estados = num_estados;
	afnd->estados=(Estado**)malloc(sizeof(Estado*)*num_estados);
	afnd->num_estados_actual = 0;
	afnd->cadena_actual = palabraNueva();
	afnd->estados_actuales = (int *)calloc(num_estados,sizeof(int));
	return afnd;
}

/*
Elimina un AFND, liberando toda la memoria
*/
void AFNDElimina(AFND * p_afnd){
	for(int i = 0; i < p_afnd->num_estados; i++){
		estadoElimina(p_afnd->estados[i]);
	}
	free(p_afnd->estados);
	free(p_afnd->estados_actuales);
	free(p_afnd->nombre);
	alfabetoElimina(p_afnd->alfabeto);
	palabraElimina(p_afnd->cadena_actual);
	free(p_afnd);
}

/*
Imprime un AFND
*/
void AFNDImprime(FILE * fd, AFND* p_afnd){
	int i, j, k;
	int *destinos;
	fprintf(fd, "%s={\n", p_afnd->nombre);
	fprintf(fd, "\tnum_simbolos = %d\n\n", p_afnd->num_simbolos);
	fprintf(fd, "\t");
	alfabetoImprime(fd, p_afnd->alfabeto);
	fprintf(fd, "\n\tnum_estados = %d\n\n", p_afnd->num_estados);
	fprintf(fd, "\tQ={");
	for(i = 0; i < p_afnd->num_estados; i++){
		estadoImprime(fd, p_afnd->estados[i]);
	}
	fprintf(fd, "}\n\n\tFunción de Transición = {\n\n");
	for(i = 0; i < p_afnd->num_estados; i++){
		for(j = 0; j < p_afnd->num_simbolos; j++){
			fprintf(fd, "\t\tf(%s,%s)={ ", estadoNombre(p_afnd->estados[i]),
										   alfabetoGetSimbolo(p_afnd->alfabeto, j));
			destinos = getTransicion(p_afnd->estados[i], j);
			for(k = 0; k < p_afnd->num_estados; k++){
				if(destinos[k] == 1){
					fprintf(fd, "%s ", estadoNombre(p_afnd->estados[k]));
				}
			}
			fprintf(fd, "}\n");
		}
	}
	fprintf(fd, "\n\t}\n}\n");
	return;
}

/*
Inserta un simbolo en el AFND
*/
AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo){
	alfabetoInsertaSimbolo(p_afnd->alfabeto, simbolo);
	return p_afnd;
}


/*
Inserta un estado en el AFND
*/
AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo){
	p_afnd->estados[p_afnd->num_estados_actual] = estadoNuevo(nombre, tipo, p_afnd->num_estados, p_afnd->num_simbolos);
	p_afnd->num_estados_actual++;
	return p_afnd;
}

/*
Inserta una transicion al AFND
*/
AFND * AFNDInsertaTransicion(AFND * p_afnd, 
                             char * nombre_estado_i, 
                             char * nombre_simbolo_entrada, 
                             char * nombre_estado_f ){
	int indice_i, indice_f, indice_simbolo;
	for( int i = 0; i < p_afnd->num_estados; i++){
		if(estadoEs(p_afnd->estados[i], nombre_estado_i))
			indice_i = i;
		if(estadoEs(p_afnd->estados[i], nombre_estado_f))
			indice_f = i;
	}
	indice_simbolo = getIndice(p_afnd->alfabeto, nombre_simbolo_entrada);

	estadoInsertaTransicion(p_afnd->estados[indice_i], indice_simbolo, indice_f);
	return p_afnd;
}

/*
Inserta una letra en el AFND
*/
AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra){
	palabraInsertaLetra(p_afnd->cadena_actual, letra);
	return p_afnd;
}

/*
Imprime el conjunto de estados actuales del AFND
*/
void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd){
	fprintf(fd, "ACTUALMENTE EN {");
	for(int i = 0; i < p_afnd->num_estados; i++){
		if(p_afnd->estados_actuales[i] == 1){
			estadoImprime(fd, p_afnd->estados[i]);
		}
	}
	fprintf(fd, "}\n");
}

/*
Imprime la cadena actual del AFND
*/
void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd){
	palabraImprime(fd, p_afnd->cadena_actual);
}

/*
Inicializa el estado de un AFND
*/
AFND * AFNDInicializaEstado (AFND * p_afnd){
	int estado;
	for(int i = 0; i < p_afnd->num_estados; i++){
		estado = estadoTipo(p_afnd->estados[i]);
		if(estado == INICIAL || estado == INICIAL_Y_FINAL)
			p_afnd->estados_actuales[i] = 1;
		else
			p_afnd->estados_actuales[i] = 0;
	}
	return p_afnd;
}

/*
Procesa la entrada segun el AFND
*/
void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd){
	AFNDInicializaEstado(p_afnd);
	AFNDImprimeConjuntoEstadosActual(fd, p_afnd);
	AFNDImprimeCadenaActual(fd, p_afnd);
	while(palabraTamano(p_afnd->cadena_actual) != 0){
		AFNDTransita(p_afnd);
		AFNDImprimeConjuntoEstadosActual(fd, p_afnd);
		AFNDImprimeCadenaActual(fd, p_afnd);
	}
}

/*
Hace que el AFND transite
*/
void AFNDTransita(AFND * p_afnd){
	int indice_letra;
	char* letra = palabraQuitaInicio(p_afnd->cadena_actual);
	int* transiciones;
	int* aux = (int*)calloc(sizeof(int), p_afnd->num_estados);
	int k;
	for( int i = 0; i < p_afnd->num_estados; i++){
		if(p_afnd->estados_actuales[i] == 1){
			indice_letra = getIndice(p_afnd->alfabeto, letra);
			transiciones = getTransicion(p_afnd->estados[i], indice_letra);
			for(k = 0; k < p_afnd->num_estados; k++){
				if(transiciones[k] == 1){
					aux[k] = 1;
				}
			}
		}
	}
	free(p_afnd->estados_actuales);
	free(letra);
	p_afnd->estados_actuales = aux;
}

