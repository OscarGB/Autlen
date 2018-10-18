#include <stdio.h>

#include "alfabeto.h"


int main(int argc, char ** argv)
{
	Alfabeto * binario;

	binario = alfabetoNuevo("0_1",2);

	alfabetoInsertaSimbolo(binario,"el_0");
	alfabetoImprime(stdout, binario);

	alfabetoInsertaSimbolo(binario,"el_1");
	alfabetoImprime(stdout, binario);


	alfabetoElimina(binario);

	return 0;
}