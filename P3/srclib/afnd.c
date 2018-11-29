
#include "afnd.h"

#include "palabra.h"
#include "alfabeto.h"
#include "estado.h"

#include <math.h>

/*Variable global para no repetir nombres de estados*/
int indiceaux = 0;

struct _AFND {
    char * nombre;
    Alfabeto * alfabeto;
    int num_estados;
    int num_simbolos;
    int num_estados_actual;
    int * estados_actuales;
    Estado ** estados;
    Palabra * cadena_actual;
    int ** lambdas;
    int o1; /*Flag para saber si es de un output*/
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
	afnd->lambdas = (int **)calloc(num_estados,sizeof(int*));
	for (int i = 0; i<num_estados; i++){
		afnd->lambdas[i] = (int *)calloc(num_estados,sizeof(int));
		afnd->lambdas[i][i] = 1;
	}
	afnd->o1 = 0;
	return afnd;
}

/*
Elimina un AFND, liberando toda la memoria
*/
void AFNDElimina(AFND * p_afnd){
	for(int i = 0; i < p_afnd->num_estados; i++){
		free(p_afnd->lambdas[i]);
		estadoElimina(p_afnd->estados[i]);
	}
	free(p_afnd->lambdas);
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
	fprintf(fd, "}\n\n\tRL++*{\n\t\t");
	for(i = 0; i < p_afnd->num_estados; i++){
		fprintf(fd, "\t[%d]", i);
	}
	for(j = 0; j < p_afnd->num_estados; j++){
		fprintf(fd, "\n\t\t[%d]", j);
		for(i = 0; i < p_afnd->num_estados; i++){
			fprintf(fd, "\t%d", p_afnd->lambdas[j][i]);
		}
	}
	fprintf(fd, "\n\t}\n\n\tFunción de Transición = {\n\n");
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
Inserta una transicion lambda al AFND
*/
AFND * AFNDInsertaLTransicion(AFND * p_afnd, 
                             char * nombre_estado_i,  
                             char * nombre_estado_f ){
	int indice_i, indice_f;
	for( int i = 0; i < p_afnd->num_estados; i++){
		if(estadoEs(p_afnd->estados[i], nombre_estado_i))
			indice_i = i;
		if(estadoEs(p_afnd->estados[i], nombre_estado_f))
			indice_f = i;
	}
	p_afnd->lambdas[indice_i][indice_f] = 1;
	return p_afnd;
}

/*
Calcula la matriz de transiciones lambda
*/
void AFNDCierraLTransicion(AFND * p_afnd){
	int ** A = NULL;
	int j, k, l;
	for( int i = 0; i <= log2(p_afnd->num_estados); i++){
		/*Copia matriz*/
		A = (int **)calloc(p_afnd->num_estados, sizeof(int*));
		for(j = 0; j< p_afnd->num_estados; j++){
			A[j] = (int *)calloc(p_afnd->num_estados, sizeof(int));
			for(k=0; k < p_afnd->num_estados; k++){
				A[j][k] = p_afnd->lambdas[j][k];
			}
		}
		/*Fin copia matriz*/
		/*Multiplicacion matriz*/
		for (j=0; j<p_afnd->num_estados; j++){
			for (k=0; k<p_afnd->num_estados; k++){
				p_afnd->lambdas[j][k] = 0; 
				for (l=0; l<p_afnd->num_estados; l++){
					if(p_afnd->lambdas[j][k] == 0){
						p_afnd->lambdas[j][k] += A[j][l] * A[l][k];					
					}
					else{
						break;
					}
				}
			}
		}
		/*Fin multiplicacion matriz*/
		/*Libera matriz*/
		for (int i = 0; i<p_afnd->num_estados; i++){
			free(A[i]);
		}
		free(A);
		/*Fin libera matriz*/
	}
}

/*
Inserta una letra en el AFND
*/
AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra){
	palabraInsertaLetra(p_afnd->cadena_actual, letra);
	return p_afnd;
}

/*
No hace nada, pero nos lo piden
*/
AFND * AFNDInicializaCadenaActual(AFND * p_afnd_l){
	return p_afnd_l;
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
	int flag = 0, i;
	AFNDInicializaEstado(p_afnd);
	AFNDImprimeConjuntoEstadosActual(fd, p_afnd);
	AFNDImprimeCadenaActual(fd, p_afnd);
	while(palabraTamano(p_afnd->cadena_actual) != 0){
		flag = 0;
		for(i = 0; i < p_afnd->num_estados;i++){
			flag = flag || p_afnd->estados_actuales[i];
		}
		if(flag == 0){
			palabraElimina(p_afnd->cadena_actual);
			p_afnd->cadena_actual = palabraNueva();
			break;
		}
		AFNDTransita(p_afnd);
		AFNDImprimeConjuntoEstadosActual(fd, p_afnd);
		AFNDImprimeCadenaActual(fd, p_afnd);
	}
}

/*
Transita las lambdas
*/
void AFNDLTransita(AFND * p_afnd){
	int i,j;
	int * A = NULL;

	A = (int *)calloc(p_afnd->num_estados, sizeof(int));
	for(j = 0; j< p_afnd->num_estados; j++){
		A[j] = p_afnd->estados_actuales[j];
	}

	for(i = 0; i < p_afnd->num_estados; i++){
		p_afnd->estados_actuales[i] = 0;
		for(j = 0; j < p_afnd->num_estados; j++){
			if(p_afnd->estados_actuales[i] == 0){
				p_afnd->estados_actuales[i] += A[j] * p_afnd->lambdas[j][i];					
			}
			else{
				break;
			}
		}
	}

	free(A);
}

/*
Hace que el AFND transite
*/
void AFNDTransita(AFND * p_afnd){
	int indice_letra;
	char* letra = palabraQuitaInicio(p_afnd->cadena_actual);
	if(getIndice(p_afnd->alfabeto, letra) < 0){
		for(int i = 0; i < p_afnd->num_estados; i++){
			p_afnd->estados_actuales[i] = 0;
			return;
		}
	}
	int* transiciones;
	int* aux = (int*)calloc(sizeof(int), p_afnd->num_estados);
	int k;
	AFNDLTransita(p_afnd);
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
	AFNDLTransita(p_afnd);
}

/*
Crea un nuevo automata con un solo simbolo
*/
AFND * AFND1ODeSimbolo(char * simbolo){
	AFND * nuevo = NULL;
	char nombre1[10], nombre2[10];

	sprintf(nombre1, "q%d", indiceaux++);
	sprintf(nombre2, "q%d", indiceaux++);

	nuevo = AFNDNuevo("afnd", 2, 1);
	nuevo->o1 = 1;

	AFNDInsertaSimbolo(nuevo, simbolo);

	AFNDInsertaEstado(nuevo, nombre1, INICIAL);
	AFNDInsertaEstado(nuevo, nombre2, FINAL);


	AFNDInsertaTransicion(nuevo, nombre1, simbolo, nombre2);

	return nuevo;
}

/*
Crea un nuevo automata con un lambda
*/
AFND * AFND1ODeLambda(){
	AFND * nuevo = NULL;
	char nombre1[10], nombre2[10];

	sprintf(nombre1, "q%d", indiceaux++);
	sprintf(nombre2, "q%d", indiceaux++);

	nuevo = AFNDNuevo("afnd", 2, 1);
	nuevo->o1 = 1;

	AFNDInsertaEstado(nuevo, nombre1, INICIAL);
	AFNDInsertaEstado(nuevo, nombre2, FINAL);


	AFNDInsertaLTransicion(nuevo, nombre1, nombre2);

	return nuevo;
}

/*
Crea un nuevo automata vacio
*/
AFND * AFND1ODeVacio(){
	AFND * nuevo = NULL;
	char nombre1[10], nombre2[10];

	sprintf(nombre1, "q%d", indiceaux++);
	sprintf(nombre2, "q%d", indiceaux++);

	nuevo = AFNDNuevo("afnd", 2, 1);
	nuevo->o1 = 1;

	AFNDInsertaEstado(nuevo, nombre1, INICIAL);
	AFNDInsertaEstado(nuevo, nombre2, FINAL);

	return nuevo;
}

/*
Transforma cualquier AFND a 1O*/
AFND * AFNDAAFND1O(AFND * p_afnd1){
	if(p_afnd1->o1) return p_afnd1;

	AFND * nuevo = NULL;
	char nombre1[10], nombre2[10];
	char ** simbolos1 = alfabetoGetTodosSimbolos(p_afnd1->alfabeto);
	char * simbolo_actual, * nombre_aux;
	int i = 0, j = 0, k = 0;
	int * transiciones;

	simbolo_actual = simbolos1[i++];

	int nuevo_num_simbolos = p_afnd1->num_simbolos;
	int nuevo_num_estados = p_afnd1->num_estados + 2;
	printf("%d\n\n\n", nuevo_num_estados);

	nuevo = AFNDNuevo("afnd", nuevo_num_estados, nuevo_num_simbolos);
	nuevo->o1 = 1;

	while(simbolo_actual){
		AFNDInsertaSimbolo(nuevo, simbolo_actual);

		simbolo_actual = simbolos1[i++];
	}

	/*Insertar estados de los AFND dados*/
	for(i = 0; i < p_afnd1->num_estados; i++){
		AFNDInsertaEstado(nuevo, estadoNombre(p_afnd1->estados[i]), NORMAL);
	}

	sprintf(nombre1, "q%d", indiceaux++);
	sprintf(nombre2, "q%d", indiceaux++);
	
	AFNDInsertaEstado(nuevo, nombre1, INICIAL);
	AFNDInsertaEstado(nuevo, nombre2, FINAL);

	/*Insertamos las transiciones*/
	for(i = 0; i < p_afnd1->num_estados; i++){
		nombre_aux = estadoNombre(p_afnd1->estados[i]);
		for(j = 0; j < p_afnd1->num_simbolos; j++){
			simbolo_actual = alfabetoGetSimbolo(p_afnd1->alfabeto, j);
			transiciones = getTransicion(p_afnd1->estados[i], j);
			for(k = 0; k < p_afnd1->num_estados; k++){
				if(transiciones[k] == 1){
					AFNDInsertaTransicion(nuevo, nombre_aux, simbolo_actual, estadoNombre(p_afnd1->estados[k]));
				}
			}
		}
	}

	for(i = 0; i < p_afnd1->num_estados; i++){
		if(estadoTipo(p_afnd1->estados[i]) == INICIAL || estadoTipo(p_afnd1->estados[i]) == INICIAL_Y_FINAL){
			nuevo->lambdas[nuevo->num_estados-2][i] = 1;
		}
		if(estadoTipo(p_afnd1->estados[i]) == FINAL || estadoTipo(p_afnd1->estados[i]) == INICIAL_Y_FINAL){
			nuevo->lambdas[i][nuevo->num_estados-1] = 1;
		}
		for(j = 0; j <  p_afnd1->num_estados; j++){
			nuevo->lambdas[i][j] = p_afnd1->lambdas[i][j];
		}
	}

	return nuevo;

}

/*
Crea un afnd a partir de dos dados, uniendolos
*/
AFND * AFND1OUne(AFND * p_afnd1O_1, AFND * p_afnd1O_2){
	AFND * nuevo = NULL;
	char nombre1[10], nombre2[10];
	char ** simbolos1 = alfabetoGetTodosSimbolos(p_afnd1O_1->alfabeto);
	char ** simbolos2 = alfabetoGetTodosSimbolos(p_afnd1O_2->alfabeto);
	char * simbolo_actual, * nombre_aux;
	int i = 0, j = 0, k = 0;
	AFND * p_afnd1, * p_afnd2;
	int * transiciones;
	int repetidos = 0;

	for (i = 0; i < p_afnd1O_1->num_simbolos; i++){
		printf("%s %d\n", simbolos1[i], getIndice(p_afnd1O_2->alfabeto, simbolos1[i]));
		if(getIndice(p_afnd1O_2->alfabeto, simbolos1[i]) >= 0){
			repetidos++;
		}
	}

	simbolo_actual = simbolos1[i++];

	p_afnd1 = AFNDAAFND1O(p_afnd1O_1);
	p_afnd2 = AFNDAAFND1O(p_afnd1O_2);

	int nuevo_num_simbolos = p_afnd1->num_simbolos + p_afnd2->num_simbolos - repetidos;
	int nuevo_num_estados = p_afnd1->num_estados + p_afnd2->num_estados + 2;

	nuevo = AFNDNuevo("afnd", nuevo_num_estados, nuevo_num_simbolos);
	nuevo->o1 = 1;

	while(simbolo_actual){
		AFNDInsertaSimbolo(nuevo, simbolo_actual);

		simbolo_actual = simbolos1[i++];
	}

	i = 0;
	simbolo_actual = simbolos2[i++];
	while(simbolo_actual){

		if(getIndice(nuevo->alfabeto, simbolo_actual) < 0){
			AFNDInsertaSimbolo(nuevo, simbolo_actual);
		}

		simbolo_actual = simbolos2[i++];		
	}


	/*Insertar estados de los AFND dados*/
	for(i = 0; i < p_afnd1->num_estados; i++){
		AFNDInsertaEstado(nuevo, estadoNombre(p_afnd1->estados[i]), NORMAL);
	}

	for(i = 0; i < p_afnd2->num_estados; i++){
		AFNDInsertaEstado(nuevo, estadoNombre(p_afnd2->estados[i]), NORMAL);
	}

	sprintf(nombre1, "q%d", indiceaux++);
	sprintf(nombre2, "q%d", indiceaux++);
	
	AFNDInsertaEstado(nuevo, nombre1, INICIAL);
	AFNDInsertaEstado(nuevo, nombre2, FINAL);



	/*Insertamos las transiciones*/
	for(i = 0; i < p_afnd1->num_estados; i++){
		nombre_aux = estadoNombre(p_afnd1->estados[i]);
		for(j = 0; j < p_afnd1->num_simbolos; j++){
			simbolo_actual = alfabetoGetSimbolo(p_afnd1->alfabeto, j);
			transiciones = getTransicion(p_afnd1->estados[i], j);
			for(k = 0; k < p_afnd1->num_estados; k++){
				if(transiciones[k] == 1){
					AFNDInsertaTransicion(nuevo, nombre_aux, simbolo_actual, estadoNombre(p_afnd1->estados[k]));
				}
			}
		}
	}

	for(i = 0; i < p_afnd2->num_estados; i++){
		nombre_aux = estadoNombre(p_afnd2->estados[i]);
		for(j = 0; j < p_afnd2->num_simbolos; j++){
			simbolo_actual = alfabetoGetSimbolo(p_afnd2->alfabeto, j);
			transiciones = getTransicion(p_afnd2->estados[i], j);
			for(k = 0; k < p_afnd2->num_estados; k++){
				if(transiciones[k] == 1){
					AFNDInsertaTransicion(nuevo, nombre_aux, simbolo_actual, estadoNombre(p_afnd2->estados[k]));
				}
			}
		}
	}

	for(i = 0; i < p_afnd1->num_estados; i++){
		for(j = 0; j <  p_afnd1->num_estados; j++){
			nuevo->lambdas[i][j] = p_afnd1->lambdas[i][j];
		}
	}
	for(i = 0; i < p_afnd2->num_estados; i++){
		for(j = 0; j <  p_afnd2->num_estados; j++){
			nuevo->lambdas[i+p_afnd1->num_estados][j+p_afnd1->num_estados] = p_afnd2->lambdas[i][j];
		}
	}

	nuevo->lambdas[nuevo->num_estados - 2][p_afnd1->num_estados - 2] = 1;
	nuevo->lambdas[nuevo->num_estados - 2][nuevo->num_estados - 4] = 1;
	nuevo->lambdas[p_afnd1->num_estados - 1][nuevo->num_estados - 1] = 1;
	nuevo->lambdas[nuevo->num_estados - 3][nuevo->num_estados - 1] = 1;


	if(p_afnd1O_1->o1 == 0)
		AFNDElimina(p_afnd1);

	if(p_afnd1O_2->o1 == 0)
		AFNDElimina(p_afnd2);

	return nuevo;
}

/*
Crea un afnd a partir de dos dados, concatenandolos
*/
AFND * AFND1OConcatena(AFND * p_afnd1O_1, AFND * p_afnd1O_2){
	AFND * nuevo = NULL;
	char nombre1[10], nombre2[10];
	char ** simbolos1 = alfabetoGetTodosSimbolos(p_afnd1O_1->alfabeto);
	char ** simbolos2 = alfabetoGetTodosSimbolos(p_afnd1O_2->alfabeto);
	char * simbolo_actual, * nombre_aux;
	int i = 0, j = 0, k = 0;
	AFND * p_afnd1, * p_afnd2;
	int * transiciones;
	int repetidos = 0;

	for (i = 0; i < p_afnd1O_1->num_simbolos; i++){
		printf("%s %d\n", simbolos1[i], getIndice(p_afnd1O_2->alfabeto, simbolos1[i]));
		if(getIndice(p_afnd1O_2->alfabeto, simbolos1[i]) >= 0){
			repetidos++;
			printf("%s esta repetido\n", simbolos1[i]);
		}
	}


	simbolo_actual = simbolos1[i++];

	p_afnd1 = AFNDAAFND1O(p_afnd1O_1);
	p_afnd2 = AFNDAAFND1O(p_afnd1O_2);

	int nuevo_num_simbolos = p_afnd1->num_simbolos + p_afnd2->num_simbolos - repetidos;
	int nuevo_num_estados = p_afnd1->num_estados + p_afnd2->num_estados + 2;

	nuevo = AFNDNuevo("afnd", nuevo_num_estados, nuevo_num_simbolos);
	nuevo->o1 = 1;

	while(simbolo_actual){
		AFNDInsertaSimbolo(nuevo, simbolo_actual);

		simbolo_actual = simbolos1[i++];
	}

	i = 0;
	simbolo_actual = simbolos2[i++];
	while(simbolo_actual){

		if(getIndice(nuevo->alfabeto, simbolo_actual) < 0){
			AFNDInsertaSimbolo(nuevo, simbolo_actual);
		}

		simbolo_actual = simbolos2[i++];		
	}


	/*Insertar estados de los AFND dados*/
	for(i = 0; i < p_afnd1->num_estados; i++){
		AFNDInsertaEstado(nuevo, estadoNombre(p_afnd1->estados[i]), NORMAL);
	}

	for(i = 0; i < p_afnd2->num_estados; i++){
		AFNDInsertaEstado(nuevo, estadoNombre(p_afnd2->estados[i]), NORMAL);
	}

	sprintf(nombre1, "q%d", indiceaux++);
	sprintf(nombre2, "q%d", indiceaux++);
	
	AFNDInsertaEstado(nuevo, nombre1, INICIAL);
	AFNDInsertaEstado(nuevo, nombre2, FINAL);



	/*Insertamos las transiciones*/
	for(i = 0; i < p_afnd1->num_estados; i++){
		nombre_aux = estadoNombre(p_afnd1->estados[i]);
		for(j = 0; j < p_afnd1->num_simbolos; j++){
			simbolo_actual = alfabetoGetSimbolo(p_afnd1->alfabeto, j);
			transiciones = getTransicion(p_afnd1->estados[i], j);
			for(k = 0; k < p_afnd1->num_estados; k++){
				if(transiciones[k] == 1){
					AFNDInsertaTransicion(nuevo, nombre_aux, simbolo_actual, estadoNombre(p_afnd1->estados[k]));
				}
			}
		}
	}

	for(i = 0; i < p_afnd2->num_estados; i++){
		nombre_aux = estadoNombre(p_afnd2->estados[i]);
		for(j = 0; j < p_afnd2->num_simbolos; j++){
			simbolo_actual = alfabetoGetSimbolo(p_afnd2->alfabeto, j);
			transiciones = getTransicion(p_afnd2->estados[i], j);
			for(k = 0; k < p_afnd2->num_estados; k++){
				if(transiciones[k] == 1){
					AFNDInsertaTransicion(nuevo, nombre_aux, simbolo_actual, estadoNombre(p_afnd2->estados[k]));
				}
			}
		}
	}

	for(i = 0; i < p_afnd1->num_estados; i++){
		for(j = 0; j <  p_afnd1->num_estados; j++){
			nuevo->lambdas[i][j] = p_afnd1->lambdas[i][j];
		}
	}
	for(i = 0; i < p_afnd2->num_estados; i++){
		for(j = 0; j <  p_afnd2->num_estados; j++){
			nuevo->lambdas[i+p_afnd1->num_estados][j+p_afnd1->num_estados] = p_afnd2->lambdas[i][j];
		}
	}

	nuevo->lambdas[nuevo->num_estados - 2][p_afnd1->num_estados - 2] = 1;
	nuevo->lambdas[p_afnd1->num_estados - 1][nuevo->num_estados - 4] = 1;
	nuevo->lambdas[nuevo->num_estados - 3][nuevo->num_estados - 1] = 1;


	if(p_afnd1O_1->o1 == 0)
		AFNDElimina(p_afnd1);

	if(p_afnd1O_2->o1 == 0)
		AFNDElimina(p_afnd2);

	return nuevo;
}

/*
Crea un afnd a partir de uno dado, aplicando la operación asterisco
*/
AFND * AFND1OEstrella(AFND * p_afnd1O_1){
	AFND * nuevo = NULL;
	char nombre1[10], nombre2[10];
	char ** simbolos1 = alfabetoGetTodosSimbolos(p_afnd1O_1->alfabeto);
	char * simbolo_actual, * nombre_aux;
	int i = 0, j = 0, k = 0;
	AFND * p_afnd1;
	int * transiciones;

	simbolo_actual = simbolos1[i++];

	p_afnd1 = AFNDAAFND1O(p_afnd1O_1);

	int nuevo_num_simbolos = p_afnd1->num_simbolos;
	int nuevo_num_estados = p_afnd1->num_estados + 2;

	nuevo = AFNDNuevo("afnd", nuevo_num_estados, nuevo_num_simbolos);
	nuevo->o1 = 1;

	while(simbolo_actual){
		AFNDInsertaSimbolo(nuevo, simbolo_actual);

		simbolo_actual = simbolos1[i++];
	}

	/*Insertar estados de los AFND dados*/
	for(i = 0; i < p_afnd1->num_estados; i++){
		AFNDInsertaEstado(nuevo, estadoNombre(p_afnd1->estados[i]), NORMAL);
	}

	sprintf(nombre1, "q%d", indiceaux++);
	sprintf(nombre2, "q%d", indiceaux++);
	
	AFNDInsertaEstado(nuevo, nombre1, INICIAL);
	AFNDInsertaEstado(nuevo, nombre2, FINAL);



	/*Insertamos las transiciones*/
	for(i = 0; i < p_afnd1->num_estados; i++){
		nombre_aux = estadoNombre(p_afnd1->estados[i]);
		for(j = 0; j < p_afnd1->num_simbolos; j++){
			simbolo_actual = alfabetoGetSimbolo(p_afnd1->alfabeto, j);
			transiciones = getTransicion(p_afnd1->estados[i], j);
			for(k = 0; k < p_afnd1->num_estados; k++){
				if(transiciones[k] == 1){
					AFNDInsertaTransicion(nuevo, nombre_aux, simbolo_actual, estadoNombre(p_afnd1->estados[k]));
				}
			}
		}
	}

	for(i = 0; i < p_afnd1->num_estados; i++){
		for(j = 0; j <  p_afnd1->num_estados; j++){
			nuevo->lambdas[i][j] = p_afnd1->lambdas[i][j];
		}
	}

	nuevo->lambdas[nuevo->num_estados - 2][nuevo->num_estados - 4] = 1;
	nuevo->lambdas[nuevo->num_estados - 1][nuevo->num_estados - 2] = 1;
	nuevo->lambdas[nuevo->num_estados - 2][nuevo->num_estados - 1] = 1;
	nuevo->lambdas[nuevo->num_estados - 3][nuevo->num_estados - 1] = 1;


	if(p_afnd1O_1->o1 == 0)
		AFNDElimina(p_afnd1);

	return nuevo;
}