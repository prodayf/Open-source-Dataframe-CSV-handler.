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
    int contador = 0;
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
        prompt2(df);
        (&lista, 0, aactivo);
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
            prompt2(df);
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
                
                // imprimirLista(&lista);
                defaultName(df, numeroDataframes(&lista));
                insertarDataframeLista(&lista, df);
                prompt2(df);
                // (&lista, 0, aactivo);
                numeroDataframes(&lista);
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
            }
            else
            {
                delcolumn(df, token);
                printf("Columna eliminada correctamente.\n");
                prompt2(df);
                (&lista, 0, aactivo);
                imprimirDataframe(df);
            }
        }

        else if (strncmp(comando, "view ", 4) == 0)
        {
            char *token = strtok(comando, " "); // Divide el comando inicial
            token = strtok(NULL, " ");          // Intenta obtener el siguiente token (argumento)
            int indice = atoi(token);
            if (token == NULL)
            {
                establecer_color(ROJO);
                printf("Falta el nombre de la columna.\n");
                continue;
            }
            else
            {
                view(df, indice);
                prompt2(df);
                (&lista, 0, aactivo);
            }
        }

        else if (strncmp(comando, "delnull ", 7) == 0)
        {
            char *token = strtok(comando, " "); // Divide el comando inicial
            token = strtok(NULL, " ");          // Intenta obtener el siguiente token (argumento)
            if (token == NULL)
            {
                establecer_color(ROJO);
                printf("Falta el nombre de la columna.\n");
                continue;
            }
            else
            {
                delnull(df, token);
                imprimirDataframe(df);
                prompt2(df);
                (&lista, 0, aactivo);
            }
        }

        else if (strncmp(comando, "save ", 4) == 0)
        {
            char *token = strtok(comando, " "); // Divide el comando inicial
            token = strtok(NULL, " ");          // Intenta obtener el siguiente token (argumento)
            if (token == NULL)
            {
                establecer_color(ROJO);
                printf("falta el nombre del fichero\n");
                continue;
            }
            else
            {
                save(df, token);
                prompt2(df);
                (&lista, 0, aactivo);
            }
        }
        else if (strncmp(comando, "quarter ", 7) == 0)
        {
            char *token = strtok(comando, " "); // Divide el comando inicial (comando: "quarter")
            token = strtok(NULL, " ");          // Obtiene el nombre de la columna existente

            if (token == NULL)
            {
                establecer_color(ROJO);
                printf("Falta el nombre de la columna existente (de tipo FECHA).\n");
                continue;
            }

            char *nombreColumna = token; // Almacena el nombre de la columna existente

            token = strtok(NULL, " "); // Obtiene el nombre de la nueva columna
            if (token == NULL)
            {
                establecer_color(ROJO);
                printf("Falta el nombre de la nueva columna.\n");
                continue;
            }

            char *nombreNuevaColumna = token; // Almacena el nombre de la nueva columna

            // Llamada a la función quarter con los dos parámetros obtenidos
            quarter(df, nombreColumna, nombreNuevaColumna);
            imprimirDataframe(df);

            // Llamar al prompt2(df); después de ejecutar el comando
            prompt2(df);
            (&lista, 0, aactivo);
        }

        else if (strncmp(comando, "list", 4) == 0)
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
            list2(&lista);
        }

        else if (strncmp(comando, "filter", 6) == 0)
        {
            char *token = strtok(comando, " "); // Divide el comando inicial
            token = strtok(NULL, " ");          // Obtiene el nombre de la columna

            if (token == NULL)
            {
                establecer_color(ROJO);
                printf("Error: Falta el nombre de la columna para filtrar.\n");
                continue;
            }
            char nombre_columna[30];
            strcpy(nombre_columna, token);

            token = strtok(NULL, " "); // Obtiene el operador
            if (token == NULL)
            {
                establecer_color(ROJO);
                printf("Error: Falta el operador para filtrar.\n");
                continue;
            }
            char operador[10];
            strcpy(operador, token);

            token = strtok(NULL, " "); // Obtiene el valor
            if (token == NULL)
            {
                establecer_color(ROJO);
                printf("Error: Falta el valor para filtrar.\n");
                continue;
            }
            char valor[30];
            strcpy(valor, token);

            token = strtok(NULL, " "); // Verifica que no haya más parámetros
            if (token != NULL)
            {
                establecer_color(ROJO);
                printf("Error: Demasiados parámetros para el comando 'filter'.\n");
                continue;
            }

            // Llamada a la función de filtrado
            filter_dataframe(df, nombre_columna, operador, valor);
        }
        else if (strncmp(comando, "prefix", 6) == 0)
        {
            char *token = strtok(comando, " "); // Divide el comando inicial
            token = strtok(NULL, " ");          // Obtiene el nombre de la columna

            if (token == NULL)
            {
                establecer_color(ROJO);
                printf("Error: Falta el nombre de la columna.\n");
                continue;
            }
            char nombre_columna[30];
            strcpy(nombre_columna, token);

            token = strtok(NULL, " "); // Obtiene el valor de n
            if (token == NULL)
            {
                establecer_color(ROJO);
                printf("Error: Falta el valor de 'n'.\n");
                continue;
            }

            int n = atoi(token); // Convertir el valor de 'n' a un entero
            if (n <= 0)
            {
                establecer_color(ROJO);
                printf("Error: El valor de 'n' debe ser mayor que 0.\n");
                continue;
            }

            token = strtok(NULL, " "); // Obtiene el nombre de la nueva columna
            if (token == NULL)
            {
                establecer_color(ROJO);
                printf("Error: Falta el nombre de la nueva columna.\n");
                continue;
            }
            char nueva_columna[30];
            strcpy(nueva_columna, token);

            token = strtok(NULL, " "); // Verifica que no haya más parámetros
            if (token != NULL)
            {
                establecer_color(ROJO);
                printf("Error: Demasiados parámetros para el comando 'prefix'.\n");
                continue;
            }

            // Llamada a la función para agregar la columna con prefijos
            prefix(df, nombre_columna, n, nueva_columna);
            imprimirDataframe(df);
        }
        else if (strncmp(comando, "sort", 4) == 0)
        {
            char *token = strtok(comando, " "); // Divide el comando inicial
            token = strtok(NULL, " ");          // Obtiene el nombre de la columna

            if (token == NULL)
            {
                establecer_color(ROJO);
                printf("Error: Falta el nombre de la columna.\n");
                continue;
            }

            char nombre_columna[30];
            strcpy(nombre_columna, token);

            token = strtok(NULL, " "); // Obtiene el orden (asc o des, opcional)
            char orden[4] = "asc";     // Ascendente por defecto
            if (token != NULL)
            {
                if (strcmp(token, "asc") == 0 || strcmp(token, "des") == 0)
                {
                    strcpy(orden, token);
                }
                else
                {
                    establecer_color(ROJO);
                    printf("Error: Orden inválido. Use 'asc' o 'des'.\n");
                    continue;
                }
            }

            token = strtok(NULL, " "); // Verifica que no haya más parámetros
            if (token != NULL)
            {
                establecer_color(ROJO);
                printf("Error: Demasiados parámetros para el comando 'sort'.\n");
                continue;
            }

            // Llamada a la función para ordenar el DataFrame
            sort(df, nombre_columna, orden);
            imprimirDataframe(df);
        }
        else if (strncmp(comando, "name", 4) == 0)
        {
            char *token = strtok(comando, " "); // Divide el comando inicial
            token = strtok(NULL, " ");          // Obtiene el nuevo nombre

            if (token == NULL)
            {
                printf("Error: Debes proporcionar un nombre.\n");
            }
            else if (strlen(token) > 50)
            {
                printf("Error: El nombre no puede exceder 50 caracteres.\n");
            }
            else
            {
                name(df, token);
                imprimirDataframe(df);
            }

             token = strtok(NULL, " "); // Verifica que no haya más parámetros
            if (token != NULL)
            {
                establecer_color(ROJO);
                printf("Error: Demasiados parámetros para el comando 'name'.\n");
                continue;
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