#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "lib.h"
#define ROJO 12   // Color rojo
#define VERDE 10  // Color verde
#define BLANCO 15 // Color blanco

int main()
{
    char comando[100];
    char *texto = "pepe";          // Valor numérico
    char *numeros[] = {"23","45", "67"}; // Datos para columna NUMERICO
    char *nombres[] = {"Alice", "Bob", "Charlie"}; // Datos para columna TEXTO
    unsigned char esNulo[] = {0, 0, 0}; // Ningún valor es nulo

    Columna *col = crearColumna("Nombres", detectar_tipo(nombres[0]), nombres[0], esNulo, 3);
    Columna *col2 = crearColumna("Edad", detectar_tipo(numeros[1]), numeros[1], esNulo, 3);
    Dataframe *df = crearDataframe(2, 3);

    df->columnas[0] = *col;
    df->columnas[1] = *col2;


    while (1)
    {
        establecer_color(BLANCO);
        printf("[?]:> ");
        printf("Ingrese un comando: \n");

        gets(comando);
        //detectar_tipo(comando);

        if (strcmp(comando, "1") == 0) //NOMBRES
        {
            establecer_color(VERDE);
            printf("Aleks Yuliyanov Filipov, aleks.yuliyanov@goumh.umh.es\n");
        }
        else if (strcmp(comando, "2") == 0) //IMPRIMIR COLUMNAS
        {
            establecer_color(VERDE);
            imprimirColumna(col);
            imprimirColumna(col2);
        }
        else if (strcmp(comando, "3") == 0)//ELIMINAR COLUMNAS
        {
            eliminarColumna(&col);
        }

        else if (strcmp(comando, "4") == 0)
        {
            for (int i = 0; i < df->numColumnas; i++)
            {
                imprimirDataframe(df);
            
            }
        }
    }
    return 0;
}