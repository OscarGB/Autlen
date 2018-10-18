#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estado.h"

int posEstado (Estado ** estados, char * nombre, int max)
{
    int i;

    for (i=0; i < max; i++)
    {
        if ( estadoEs(estados[i], nombre) )
        {
            return i;
        }
    }
    return -1;
}

int main (int argc, char ** argv)
{
    Estado * estados [10];
    int i;
    char nombre_estado[20];

    
    for (i=0; i< 10; i++)
    {

        sprintf(nombre_estado,"s_%d",i);
        switch ( i % 4 )
        {
            case 0:
                estados[i] = estadoNuevo(nombre_estado,INICIAL_Y_FINAL, 4, 1);
                break;
            case 1:
                estados[i] = estadoNuevo(nombre_estado,INICIAL, 4, 1);
                break;
            case 2:
                estados[i] = estadoNuevo(nombre_estado,FINAL, 4, 1);
                break;
            case 3:
                estados[i] = estadoNuevo(nombre_estado,NORMAL, 4, 1);
                break;


        }
    }

    fprintf(stdout,"El estado %s está en la posicion %d\n", argv[1], posEstado(estados, argv[1], 10 ) );


    for (i=0; i< 10; i++)
    {
        fprintf(stdout,"El tipo del estado %s es %d\n", estadoNombre(estados[i]),estadoTipo(estados[i]));
    }

    for (i=0; i< 10; i++)
    {
        estadoImprime(stdout,estados[i]);
    }


    for (i=0; i< 10; i++)
    {
        estadoElimina(estados[i]);
    }

    return 0;


}
