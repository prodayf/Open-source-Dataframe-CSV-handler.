#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "lib.h"
#define ROJO 12  // Color rojo
#define VERDE 10 // Color verde
#define BLANCO 15 // Color blanco



int main()
{
    char comando[100];


    while (1)
    {
        establecer_color(BLANCO);
        printf("[?]:> ");
        printf("Ingrese un comando: \n");
        printf("1:Imprimir en pantalla los datos del alumno: \n");
        printf("introduce una cadena:\n");
        

        gets(comando);
        detectar_tipo(comando);


        if (strcmp(comando, "1") == 0)
        {
            establecer_color(VERDE);
            printf("Aleks Yuliyanov Filipov, aleks.yuliyanov@goumh.umh.es\n");
        } 
    }
    return 0;
}