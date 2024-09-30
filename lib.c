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
    columna->tipo = tipo; // Aquí el tipo se pasa como argumento
    columna->datos = datos;
    columna->esNulo = esNulo;
    columna->numFilas = numFilas;

    if (columna == NULL && datos == NULL && esNulo == NULL && numFilas == 0)
    {
        printf("Error al crear la columna\n");
        return NULL;
    }

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
        printf("Nombre: %s, Tipo: %d, Datos: %p, esNulo: %p, NumFilas: %d\n",
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