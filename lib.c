#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "lib.h"
#define MAX_FILAS 1000
#define MAX_COLUMNAS 10
#define MAX_STRING 100
#define MAX_DATAFRAMES 100
Dataframe *dataframeActivo = NULL;


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
    // const char *tipoStr = (tipo == NUMERICO) ? "NUMERICO" : (tipo == FECHA) ? "FECHA"
    //                                                     : (tipo == TEXTO)   ? "TEXTO"
    //                                                                         : "DESCONOCIDO";

    // printf("Nombre: %s, Tipo: %s, Datos: %p, esNulo: %p, NumFilas: %d\n",
    //        columna->nombre,
    //        tipoStr,
    //        columna->datos,
    //        columna->esNulo,
    //        columna->numFilas);
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
        printf("Nombre: %s, Tipo: %d, NumFilas: %d\n",
               columna->nombre,
               columna->tipo,
               columna->numFilas);

        // Iterar sobre cada fila e imprimir el valor correspondiente
        for (int i = 0; i < columna->numFilas; i++)
        {
            if (columna->esNulo[i])
            {
                printf("Fila %d: NULL\n", i);
            }
            else
            {
                // Dado que los datos son punteros a cadenas (char**)
                printf("Fila %d: %s\n", i, ((char **)columna->datos)[i]);
            }
        }
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
    // Inicializar cada columna en el DataFrame
    for (int i = 0; i < numColumnas; i++)
    {
        df->columnas[i] = (Columna){0}; // O inicializa según tu estructura de Columna
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

// TODO: implementar la funcion correctamente ya que hay alguna especie de error,
void eliminarDataframe(Dataframe **df)
{
    if (df != NULL && *df != NULL)
    { // Verifica que el puntero y el DataFrame no sean nulos
        // Liberar cada columna en el DataFrame
        for (int i = 0; i < (*df)->numColumnas; i++)
        {
            // Liberar los datos de cada columna si no son NULL
            if ((*df)->columnas[i].datos != NULL)
            {
                free((*df)->columnas[i].datos);
                (*df)->columnas[i].datos = NULL; // Opcional, pero recomendable
            }
            // Liberar el arreglo esNulo si no es NULL
            if ((*df)->columnas[i].esNulo != NULL)
            {
                free((*df)->columnas[i].esNulo);
                (*df)->columnas[i].esNulo = NULL; // Opcional, pero recomendable
            }
        }

        // Liberar el arreglo de columnas
        free((*df)->columnas);

        // Liberar el índice
        free((*df)->indice);

        // Finalmente, liberar el DataFrame
        free(*df);
        *df = NULL; // Establecer el puntero a NULL después de liberar
    }
    else
    {
        printf("No se puede eliminar un DataFrame nulo.\n");
    }
}

void imprimirDataframe(Dataframe *df)
{
    // Comprobar si el DataFrame es nulo
    if (df == NULL)
    {
        printf("El DataFrame es nulo.\n");
        return;
    }

    // Imprimir encabezados de las columnas
    for (int i = 0; i < df->numColumnas; i++)
    {
        printf("%-30s", df->columnas[i].nombre);
    }
    printf("\n"); // Salto de línea después de imprimir los encabezados

    // Imprimir los datos de cada fila
    for (int i = 0; i < df->numFilas; i++)
    {
        for (int j = 0; j < df->numColumnas; j++)
        {
            // Suponiendo que la columna tiene un puntero a datos que almacena los valores
            if (df->columnas[j].tipo == TEXTO)
            {
                printf("%-30s", ((char **)df->columnas[j].datos)[i]); // Imprime datos de tipo texto
            }
            else if (df->columnas[j].tipo == NUMERICO)
            {
                printf("%-30s", ((char **)df->columnas[j].datos)[i]); // Imprime datos numéricos como texto
            }
            // Agrega condiciones para otros tipos de datos si es necesario
        }
        printf("\n"); // Salto de línea después de imprimir una fila
    }
}

void inicializarLista(Lista *lista)
{
    lista->numDFs = 0;
    lista->primero = NULL;
}

void insertarDataframeLista(Lista *lista, Dataframe *df)
{
    Nodo *nuevoNodo = (Nodo *)malloc(sizeof(Nodo));
    if (nuevoNodo == NULL)
    {
        printf("Error al insertar el DataFrame en la lista\n");
        return;
    }

    nuevoNodo->df = df;
    nuevoNodo->siguiente = lista->primero;
    lista->primero = nuevoNodo;
    lista->numDFs++;

    //printf("Dataframe insertado correctamente en la lista.\n");
    //imprimirDataframe(df);
}

void imprimirLista(Lista *lista)
{
    if (lista == NULL || lista->primero == NULL)
    {
        printf("La lista está vacía.\n");
        return;
    }

    Nodo *actual = lista->primero;
    while (actual != NULL)
    {
        if (actual->df != NULL)
        {
            //imprimirDataframe(actual->df);
        }
        actual = actual->siguiente;
    }

    printf("Numero  de Dataframes en la lista: %d\n", lista->numDFs);
}

void eliminarDataframeLista(Lista *lista, Dataframe *df)
{
    if (lista == NULL || lista->primero == NULL)
    {
        printf("La lista está vacía.\n");
        return;
    }

    Nodo *actual = lista->primero;
    Nodo *anterior = NULL;

    while (actual != NULL)
    {
        // Si encontramos el dataframe a eliminar
        if (actual->df == df)
        {
            // Si es el primer nodo de la lista
            if (anterior == NULL)
            {
                lista->primero = actual->siguiente;
            }
            else
            {
                anterior->siguiente = actual->siguiente;
            }

            // Libera el dataframe antes de liberar el nodo
            if (actual->df != NULL)
            {
                // eliminarDataframe(&(actual->df));  // Libera el dataframe
                actual->df = NULL;
            }

            // Libera el nodo actual
            free(actual);
            lista->numDFs--; // Decrementa el número de DataFrames en la lista
            printf("Dataframe eliminado correctamente de la lista.\n");
            return;
        }

        // Avanza al siguiente nodo
        anterior = actual;
        actual = actual->siguiente;
    }

    printf("El DataFrame no se encontró en la lista.\n");
}

void eliminarDataframes(Lista *lista)
{
    if (lista == NULL || lista->primero == NULL)
    {
        printf("La lista ya está vacía o no existe.\n");
        return;
    }

    Nodo *actual = lista->primero;
    Nodo *siguiente;

    while (actual != NULL)
    {
        siguiente = actual->siguiente; // Guarda el siguiente nodo antes de liberar

        // Libera el dataframe si existe
        if (actual->df != NULL)
        {
            actual->df = NULL; // Evita liberar el dataframe dos veces
            printf("Dataframe eliminado correctamente de la lista.\n");
        }

        // Libera el nodo actual
        free(actual);       // Aquí se libera el nodo
        actual = siguiente; // Avanza al siguiente nodo
    }

    lista->primero = NULL; // La lista queda vacía
    lista->numDFs = 0;     // Restablece el contador de DataFrames
    printf("Se han eliminado todos los dataframes de la lista.\n");
}

Dataframe* load(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return NULL;
    }

    char buffer[MAX_STRING];
    int numColumnas = 0;
    int numFilas = 0;

    //primero cuenta el numero coluumnas que hay.
    if (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char *token = strtok(buffer, ",");
        while (token != NULL)
        {
            numColumnas++;
            token = strtok(NULL, ",");
        }
    }

    // Allocate memory for the dataframe
    Dataframe *df = (Dataframe *)malloc(sizeof(Dataframe));
    df->numFilas = 0;
    df->numColumnas = numColumnas;
    df->columnas = (Columna *)malloc(numColumnas * sizeof(Columna));

    // Allocate memory for each column's datos and esNulo
    for (int i = 0; i < numColumnas; i++) {
        df->columnas[i].datos = (char **)malloc(MAX_FILAS * sizeof(char *));
        df->columnas[i].esNulo = (unsigned char *)malloc(MAX_FILAS * sizeof(unsigned char));
        df->columnas[i].numFilas = 0;
    }

    // Reset file pointer to the beginning
    rewind(file);

    // Leer la primera línea para los nombres de las columnas
    if (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char *token = strtok(buffer, ",");
        int colIndex = 0;
        while (token != NULL)
        {
            strncpy(df->columnas[colIndex].nombre, token, 30);
            colIndex++;
            token = strtok(NULL, ",");
        }
    }

    // Leer las filas restantes
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        int colIndex = 0; // lo usamos para la referencia a cada array de columnas.
        //es decir, si colindex es 0, entonces se refiere a la primera columna...
        //esto es posible porque en la estructura de la columna se tiene un array de columnas.
        char *token = strtok(buffer, ",");
        while (token != NULL && colIndex < numColumnas)
        {
            // Detectar tipo y asignar a las columnas
            TipoDato tipo = detectar_tipo(token);
            df->columnas[colIndex].tipo = tipo;

            // Asignar valor a la columna correspondiente
            if (df->columnas[colIndex].numFilas < MAX_FILAS) {
                // Cast the datos field to char** and assign the value
                ((char **)df->columnas[colIndex].datos)[df->columnas[colIndex].numFilas] = strdup(token);
                df->columnas[colIndex].esNulo[df->columnas[colIndex].numFilas] = (strlen(token) == 0); // Marcar como nulo si está vacío
                df->columnas[colIndex].numFilas++;
                //con cada iteracion se plasman los datos en la tabla utilizando el colindex para saber a que columna pertenece.



            }

            colIndex++;
            token = strtok(NULL, ",");
        }
        df->numFilas++;
    }


    imprimirDataframe(df);
    fclose(file);
    return df;
}