
#include "palabra.h"

struct _Palabra{
	char** letras; 	//Colección de símbolos (char*)
	int tamano;			//Tamaño de la palabra
};

/*
Función para crear una nueva palabra.
*/
Palabra* palabraNueva(){
	Palabra* nPal = malloc(sizeof(Palabra));
	if(!nPal) return NULL;
	nPal->tamano = 0;
	nPal->letras = NULL;
	return nPal;
}

/*
Elimina la palabra p_p.
*/
void palabraElimina(Palabra* p_p){
	if(!p_p) return;
	for(int i = 0; i < p_p->tamano; i++){
		free(p_p->letras[i]);
	}
	free(p_p->letras);
	free(p_p);
	return;
}

/*
Inserta una letra (ojo, es un char*, puede ser "alpha"
o una palabra "del lenguaje natural" entera).
*/
Palabra* palabraInsertaLetra(Palabra* p_p, char* letra){
	if(!p_p || !letra) return NULL;
	if(!p_p->letras) p_p->letras = malloc(sizeof(char*));
	else p_p->letras = realloc(p_p->letras, (p_p->tamano+1)*sizeof(char*));
	if(!p_p->letras){palabraElimina(p_p); return NULL;}
	p_p->letras[p_p->tamano] = strdup(letra);
	if(!p_p->letras[p_p->tamano]){palabraElimina(p_p); return NULL;}
	p_p->tamano++;
	return p_p;
}

/*
Imprime una palabra por el fichero indicado.
*/
void palabraImprime(FILE *fd, Palabra* p_p){
	if(!p_p || !fd) return;
	printf("[(%d)", p_p->tamano);
	for(int i = 0; i < p_p->tamano; i++){
		printf(" %s", p_p->letras[i]);
	}
	printf("]\n");
	return;
}

/*
Hace pop de la primera letra.
*/
char* palabraQuitaInicio(Palabra* p_p){
	if(!p_p) return NULL;
	char* aux = p_p->letras[0];
	for(int i = 0; i < p_p->tamano - 1; i++) {
        *(p_p->letras+i) = *(p_p->letras+i+1);
    }
    p_p->letras = realloc(p_p->letras, (p_p->tamano-1)*sizeof(char *));
	p_p->tamano--;
	return aux;
}

/*
Devuelve la letra en la posicion i
*/
char* palabraGetILetra(Palabra* p_p, int i){
	if(!p_p || i >= p_p->tamano){
		return NULL;
	}
	return p_p->letras[i];
}


/*
Devuelve el Tamaño de una palabra.
*/
int palabraTamano(Palabra* p_p){
	if(!p_p) return 0;
	return p_p->tamano;
}