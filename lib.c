#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "lib.h"

// Función para establecer el color de la consola
// Parámetros: pasa el color que esta definido en el main.c como variables globales.
void establecer_color(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// funcion que valida la fecha.
int fechaValida(char *valor)
{
    int dia, mes, anio;
    // 2d/2d/4d es que se espera que el usuario ingrese la fecha en el formato dd/mm/yyyy
    if (sscanf(valor, "%2d/%2d/%4d", &dia, &mes, &anio) == 3)
    {
        if (dia >= 1 && dia <= 31 && mes >= 1 && mes <= 12 && anio >= 1900 && anio <= 2021)
        {
            return 1;
        }
        else
        {
            printf("Fecha no valida\n");
        }
    }
    else
    {
        printf("Fecha no valida\n");
    }
}

TipoDato detectar_tipo(char *valor)
{
    // Comprobar si es un número
    if (esNumero(valor))
    {
        printf("NUMERICO\n");
        return NUMERICO;
        // tipo 0
    }

    // Comprobar si es una fecha válida
    if (fechaValida(valor))
    {
        printf("FECHA\n");
        return FECHA;

        // es tipo 2
    }

    // Si no es ni número ni fecha, es texto
    printf("TEXTO\n");
    return TEXTO;
}

int esNumero(char *valor)
{
    for (int i = 0; valor[i] != '\0'; i++)
    {
        if (valor[i] < '0' || valor[i] > '9')
        {
            return 0; // No es un número
        }
    }
    return NUMERICO; // Es un número
}

Columna *crearColumna(char nombre[30], TipoDato tipo, void *datos, unsigned char *esNulo, int numFilas)
{
    Columna *columna = (Columna *)malloc(sizeof(Columna));
    strcpy(columna->nombre, nombre);

    if (columna == NULL && datos == NULL && esNulo == NULL && numFilas == 0)
    {
        printf("Error al crear la columna\n");
        return NULL;
    }
    columna->tipo = tipo; // Aquí el tipo se pasa como argumento
    columna->datos = datos;
    columna->esNulo = esNulo;
    columna->numFilas = numFilas;

    // como es un string tiene que ser un puntero
    const char *tipoStr = (tipo == NUMERICO) ? "NUMERICO" : (tipo == FECHA) ? "FECHA"
                                                        : (tipo == TEXTO)   ? "TEXTO"
                                                                            : "DESCONOCIDO";

    printf("Nombre: %s, Tipo: %s, Datos: %p, esNulo: %p, NumFilas: %d\n",
           columna->nombre,
           tipoStr,
           columna->datos,
           columna->esNulo,
           columna->numFilas);
    return columna;
}

void eliminarColumna(Columna **columna)
{
    // el doble puntero es para que se pueda liberar la meoria y a la vez asignarle NULL
    // sin el doble puntero no se podria hacer esto.
    // porque con uno simple solo podria liberar la memoria pero no asignarle NULL
    if (columna != NULL)
    {
        // Si 'datos' apunta a memoria dinámica, libérala
        if ((*columna)->datos != NULL)
        {
            free((*columna)->datos);  // Libera memoria de datos
            (*columna)->datos = NULL; // Evitar doble liberación
        }

        if ((*columna)->esNulo != NULL)
        {
            free((*columna)->esNulo);
            (*columna)->esNulo = NULL; // Evitar doble liberación
        }

        // Libera la memoria de la columna
        free(*columna);
        *columna = NULL; // Evitar doble liberación
        printf("Columna eliminada correctamente.\n");
    }
    else
    {
        printf("No se puede eliminar una columna nula.\n");
    }
}

void imprimirColumna(Columna *columna)
{
    if (columna != NULL)
    {
        printf("Nombre: %s, Tipo: %d, Datos: %s, esNulo: %d, NumFilas: %d\n",
               columna->nombre,
               columna->tipo,
               columna->datos,
               columna->esNulo,
               columna->numFilas);
    }
    else
    {
        printf("No se puede imprimir una columna nula.\n");
    }
}

Dataframe *crearDataframe(int numColumnas, int numFilas)
{
    Dataframe *df = (Dataframe *)malloc(sizeof(Dataframe));
    if (df == NULL)
    {
        printf("Error al crear el DataFrame\n");
        return NULL;
    }
    df->numColumnas = numColumnas;
    df->numFilas = numFilas;

    // Reservar memoria para el arreglo de columnas
    df->columnas = (Columna *)malloc(numColumnas * sizeof(Columna));
    if (df->columnas == NULL)
    {
        printf("Error al reservar memoria para las columnas\n");
        free(df);
        return NULL;
    }

    // Reservar memoria para el índice
    df->indice = (int *)malloc(numFilas * sizeof(int));
    if (df->indice == NULL)
    {
        printf("Error al reservar memoria para el índice\n");
        free(df->columnas);
        free(df);
        return NULL;
    }

    for (int i = 0; i < numFilas; i++)
    {
        df->indice[i] = i; // Asignar un número de fila único
        // basicamente sierve para poder numerar las lineas de la tabla
        // nos servira tambien para accerder rapidamente a una linea en especifico
    }



    return df;
}

// void eliminarDataframe(Dataframe **df)
// {
//     if (df != NULL && *df != NULL)
//     { // Verifica que el puntero y el DataFrame no sean nulos
//         // Liberar cada columna en el DataFrame
//         for (int i = 0; i < (*df)->numColumnas; i++)
//         {
//             // Liberar los datos de cada columna si no son NULL
//             if ((*df)->columnas[i].datos != NULL)
//             {
//                 free((*df)->columnas[i].datos);
//                 (*df)->columnas[i].datos = NULL; // Opcional, pero recomendable
//             }
//             // Liberar el arreglo esNulo si no es NULL
//             if ((*df)->columnas[i].esNulo != NULL)
//             {
//                 free((*df)->columnas[i].esNulo);
//                 (*df)->columnas[i].esNulo = NULL; // Opcional, pero recomendable
//             }
//         }

//         // Liberar el arreglo de columnas
//         free((*df)->columnas);

//         // Liberar el índice
//         free((*df)->indice);

//         // Finalmente, liberar el DataFrame
//         free(*df);
//         *df = NULL; // Establecer el puntero a NULL después de liberar
//     }
//     else
//     {
//         printf("No se puede eliminar un DataFrame nulo.\n");
//     }
// }


void imprimirDataframe(Dataframe *df) {
    // Comprobar si el DataFrame es nulo
    if (df == NULL) {
        printf("El DataFrame es nulo.\n");
        return;
    }

    // Imprimir encabezados de las columnas
    for (int i = 0; i < df->numColumnas; i++) {
        printf("%-30s", df->columnas[i].nombre);
    }
    printf("\n"); // Salto de línea después de imprimir los encabezados



}