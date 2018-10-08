#include "palabra.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
        Palabra * pal;
        int i;
        char letra [20];
        char * letrai;

        pal = palabraNueva();

        for (i=0; i < argc-1; i++)
        {
                sprintf(letra,"%s",argv[1+i]);
                palabraInsertaLetra(pal,letra);
                fprintf(stdout,"pal_%d:\n",i);
                palabraImprime(stdout,pal);
                fprintf(stdout,"\n");
        }

        for (i=0; i < argc-1; i++)
        {
                if(strcmp(palabraGetILetra(pal, i), argv[i+1])!=0){
                    printf("ERROR al comprobar la letra %d, tiene que ser %s y es %s\n", i, argv[i+1], palabraGetILetra(pal, i));
                }
                else{
                    printf("Correcto\n");
                }
        }

        while ( palabraTamano(pal) > 0 )
        {
                fprintf(stdout,
    "QUITAMOS %s DE LA PALABRA QUE QUEDA ASI:\n",letrai=palabraQuitaInicio(pal));
                palabraImprime(stdout,pal);
                free(letrai);
        }

        palabraElimina(pal);

        return 0;
}
