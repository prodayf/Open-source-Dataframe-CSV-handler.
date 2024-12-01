#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "lib.h"
#define ROJO 12   // Color rojo
#define VERDE 10  // Color verde
#define BLANCO 15 // Color blanco
int aactivo = -1;
int main()
{
    char comando[100];
    char *texto = "pepe";                          // Valor numérico
    char *numeros[] = {"23", "45", "67"};          // Datos para columna NUMERICO
    char *nombres[] = {"Alice", "Bob", "Charlie"}; // Datos para columna TEXTO
    unsigned char esNulo[] = {0, 0, 0};            // Ningún valor es nulo

    // Columna *col = crearColumna("Nombres", detectar_tipo(nombres[0]), nombres, esNulo, 3);
    // Columna *col2 = crearColumna("Edad", detectar_tipo(numeros[0]), numeros, esNulo, 3);
    // Dataframe *df = crearDataframe(2, 3);
    // Dataframe *df2 = crearDataframe(2, 3);
    Dataframe *df = NULL;

    Lista lista;
    inicializarLista(&lista);

    // df->columnas[0] = *col;
    // df->columnas[1] = *col2;
    // df2->columnas[0] = *col;
    // df2->columnas[1] = *col2;

    while (1)
    {
        establecer_color(BLANCO);
        prompt(&lista, 0, aactivo);
        printf("Ingrese un comando: \n");

        gets(comando);
        // detectar_tipo(comando);

        if (strcmp(comando, "1") == 0) // NOMBRES
        {
            establecer_color(VERDE);
            printf("Aleks Yuliyanov Filipov, aleks.yuliyanov@goumh.umh.es\n");
        }
        // else if (strcmp(comando, "2") == 0) // IMPRIMIR COLUMNAS
        // {
        //     establecer_color(VERDE);
        //     imprimirColumna(col);
        //     // imprimirColumna(col2);
        // }
        // else if (strcmp(comando, "3") == 0) // ELIMINAR COLUMNAS
        // {
        //     eliminarColumna(&col);
        // }

        // else if (strcmp(comando, "4") == 0)
        // {
        //     imprimirDataframe(df);
        // }
        // else if (strcmp(comando, "5") == 0)
        // {
        //     eliminarDataframe(&df);
        //     printf("Dataframe eliminado correctamente.\n");
        // }
        // else if (strcmp(comando, "6") == 0)
        // {

        //     insertarDataframeLista(&lista, df);
        //     insertarDataframeLista(&lista, df2);
        // }
        // else if (strcmp(comando, "7") == 0)
        // {
        //     imprimirLista(&lista);
        // }

        // else if (strcmp(comando, "8") == 0)
        // {
        //     eliminarDataframeLista(&lista, df);
        // }

        // else if (strcmp(comando, "9") == 0)
        // {
        //     eliminarDataframes(&lista);
        // }

        else if (strcmp(comando, "quit") == 0)
        {
            eliminarDataframes(&lista);
            establecer_color(VERDE);
            printf("Saliendo del programa...\n");
            establecer_color(BLANCO);
            exit(0);
        }

        else if (strncmp(comando, "load", 4) == 0)
        {

            char *token = strtok(comando, " ");
            token = strtok(NULL, " ");
            if (token == NULL)
            {
                establecer_color(ROJO);
                printf("Falta el nombre del archivo.\n");
                continue;
            }
            df = load(token);
            if (df != NULL)
            {
                insertarDataframeLista(&lista, load(token));
                imprimirLista(&lista);
                prompt(&lista, 0, aactivo);
            }
            printf(token);
        }
        else if (strncmp(comando, "meta", 4) == 0)
        {
            char *token = strtok(comando, " "); // Divide el comando inicial
            token = strtok(NULL, " ");          // Intenta obtener el siguiente token (argumento)

            if (token != NULL)
            {
                // Si hay un token adicional, el comando es incorrecto
                establecer_color(ROJO);
                printf("El comando 'meta' no acepta argumentos.\n");
                continue;
            }

            // Si no hay argumentos, ejecuta meta
            meta(df);
        }

        else if (strncmp(comando, "delcolum ", 7) == 0)
        {
            char *token = strtok(comando, " "); // Divide el comando inicial
            token = strtok(NULL, " ");          // Intenta obtener el siguiente token (argumento)
            if (token == NULL)
            {
                establecer_color(ROJO);
                printf("Falta el nombre de la columna.\n");
                continue;
            }else{
                delcolumn(df, token);
                printf("Columna eliminada correctamente.\n");
                prompt(&lista, 0, aactivo);
                imprimirDataframe(df);
            }

        }
        
        


        else
        {
            establecer_color(ROJO);
            printf("Comando no reconocido.\n");
        }
    }

    return 0;
}