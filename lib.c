#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "lib.h"

// Función para establecer el color de la consola
// Parámetros: pasa el color que esta definido en el main.c como variables globales.
void establecer_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

TipoDato detectar_tipo(char *valor) {
    int esNumero = 1;

    // Bucle para comprobar los valores de la cadena.
    // \0 significa el final de la cadena.
    for (int i = 0; valor[i] != '\0'; i++) {
        if (valor[i] < '0' || valor[i] > '9') {
            esNumero = 0;
            break;
        }
    }

    // Si es un número, devuelve NUMERICO
    if (esNumero == 1) {
        printf("NUMERICO\n");
        return NUMERICO;
    } else {
        printf("TEXTO\n");
        return TEXTO;
    }
}
