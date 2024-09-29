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

// funcion que valida la fecha.
int fechaValida(char *valor){
    int dia,mes, anio; 
    // 2d/2d/4d es que se espera que el usuario ingrese la fecha en el formato dd/mm/yyyy 
    if (sscanf(valor, "%2d/%2d/%4d", &dia, &mes, &anio) == 3){
        if (dia >= 1 && dia <= 31 && mes >= 1 && mes <= 12 && anio >= 1900 && anio <= 2021){
            return 1;
        }else{
            printf("Fecha no valida\n");
        }
    }else{
        printf("Fecha no valida\n");
    }
    
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
    } 
    else if (fechaValida(valor)) {
        printf("FECHA\n");
        return FECHA;
    }else {
        printf("TEXTO\n");
        return TEXTO;
    }
    
}

//funcion para crear un dataframe 
