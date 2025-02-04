#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "lib.h"
#include <ctype.h>

#define MAX_FILAS 2000
#define MAX_COLUMNAS 10
#define MAX_STRING 100
#define MAX_DATAFRAMES 100
#define ROJO 12   // Color rojo
#define VERDE 10  // Color verde
#define BLANCO 15 // Color blanco
int activo = -1;
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
    if (sscanf(valor, "%4d/%2d/%2d", &anio, &mes, &dia) == 3)
    {
        if (dia >= 1 && dia <= 31 && mes >= 1 && mes <= 12 && anio >= 1900 && anio <= 2999)
        {
            return 1;
            printf("Fecha valida\n");
        }
        else
        {
            establecer_color(ROJO);
            printf("Fecha no valida,EL PROGARAMA SE CERRARA. el formato es YYYY-MM-DD\n");
            establecer_color(BLANCO);
            exit(EXIT_FAILURE);
        }
    }
}

TipoDato detectar_tipo(char *valor)
{
    // Comprobar si es un número
    if (esNumero(valor))
    {
        // printf("NUMERICO\n");
        return NUMERICO;
        // tipo 0
    }

    // Comprobar si es una fecha válida
    if (fechaValida(valor))
    {
        // printf("FECHA\n");
        return FECHA;

        // es tipo 2
    }

    // Si no es ni número ni fecha, es texto
    // printf("TEXTO\n");
    return TEXTO;
}

int esNumero(char *valor)
{
    int i = 0;
    int hasDecimalPoint = 0; // Bandera para verificar si ya se encontró un punto decimal

    // Verificar si el primer carácter es un signo negativo
    if (valor[0] == '-')
    {
        i++; // Saltar el signo negativo
    }

    // Recorrer el resto de la cadena
    for (; valor[i] != '\0'; i++)
    {
        if (valor[i] == '.')
        {
            // Si encontramos un punto decimal
            if (hasDecimalPoint)
            {
                return 0; // Más de un punto decimal, no es un número válido
            }
            hasDecimalPoint = 1; // Marcar que ya encontramos un punto decimal
        }
        else if (valor[i] < '0' || valor[i] > '9')
        {
            return 0; // No es un número
        }
    }

    // Si pasamos todas las verificaciones, es un número válido
    return NUMERICO;
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

void imprimirDataframe(Dataframe *activo)
{
    // Comprobar si el DataFrame activo es nulo
    if (activo == NULL)
    {
        printf("El DataFrame activo es nulo.\n");
        return;
    }
    // Imprimir encabezados de las columnas
    for (int i = 0; i < activo->numColumnas; i++)
    {
        printf("%-30s", activo->columnas[i].nombre);
    }
    printf("\n"); // Salto de línea después de imprimir los encabezados

    // Imprimir los datos de cada fila
    for (int i = 0; i < activo->numFilas; i++)
    {
        for (int j = 0; j < activo->numColumnas; j++)
        {
            if (activo->columnas[j].esNulo[i] == 1)
            {
                printf("%-30s", "NULL"); // Imprimir "NULL" para los valores nulos
            }
            else
            {
                // Suponiendo que la columna tiene un puntero a datos que almacena los valores
                if (activo->columnas[j].tipo == TEXTO)
                {
                    printf("%-30s", ((char **)activo->columnas[j].datos)[i]); // Imprime datos de tipo texto
                }
                else if (activo->columnas[j].tipo == NUMERICO)
                {
                    printf("%-30s", ((char **)activo->columnas[j].datos)[i]); // Imprime datos numéricos como texto
                }
                else if (activo->columnas[j].tipo == FECHA)
                {
                    // Si la columna es de tipo FECHA, asumimos que los datos están almacenados como cadenas
                    printf("%-30s", ((char **)activo->columnas[j].datos)[i]); // Imprime datos de tipo fecha
                }
                // Agregar más condiciones para otros tipos de datos si es necesario
            }
        }
        printf("\n"); // Salto de línea después de imprimir una fila
    }
}
// void imprimirDataframe(Dataframe *df)
// {
//     // Comprobar si el DataFrame es nulo
//     if (df == NULL)
//     {
//         printf("El DataFrame es nulo.\n");
//         return;
//     }

//     // Imprimir encabezado del índice
//     printf("%-10s", "Índice");

//     // Imprimir encabezados de las columnas
//     for (int i = 0; i < df->numColumnas; i++)
//     {
//         printf("%-30s", df->columnas[i].nombre);
//     }
//     printf("\n"); // Salto de línea después de imprimir los encabezados

//     // Imprimir los datos de cada fila
//     for (int i = 0; i < df->numFilas; i++)
//     {
//         // Imprimir el índice de la fila actual
//         printf("%-10d", df->indice[i]); // Índice correspondiente a la fila actual

//         // Imprimir los datos de cada columna
//         for (int j = 0; j < df->numColumnas; j++)
//         {
//             if (df->columnas[j].esNulo[i] == 1)
//             {
//                 printf("%-30s", "NULL"); // Imprimir "NULL" para los valores nulos
//             }
//             else
//             {
//                 // Suponiendo que la columna tiene un puntero a datos que almacena los valores
//                 if (df->columnas[j].tipo == TEXTO)
//                 {
//                     printf("%-30s", ((char **)df->columnas[j].datos)[i]); // Imprime datos de tipo texto
//                 }
//                 else if (df->columnas[j].tipo == NUMERICO)
//                 {
//                     printf("%-30s", ((char **)df->columnas[j].datos)[i]); // Imprime datos numéricos como texto
//                 }
//                 else if (df->columnas[j].tipo == FECHA)
//                 {
//                     // Si la columna es de tipo FECHA, asumimos que los datos están almacenados como cadenas
//                     printf("%-30s", ((char **)df->columnas[j].datos)[i]); // Imprime datos de tipo fecha
//                 }
//                 // Agregar más condiciones para otros tipos de datos si es necesario
//             }
//         }
//         printf("\n"); // Salto de línea después de imprimir una fila
//     }
// }

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

    // comprobar si la lista esta vacia.
    if (lista == NULL || df == NULL)
    {

        printf("Error al insertar el DataFrame en la lista\n");
        return;
    }

    nuevoNodo->df = df;
    nuevoNodo->siguiente = lista->primero;
    lista->primero = nuevoNodo;
    lista->numDFs++;

    // printf("Dataframe insertado correctamente en la lista.\n");
    imprimirDataframe(df);
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
            // imprimirDataframe(actual->df);
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

    printf("El DataFrame no se encontro en la lista.\n");
}

void eliminarDataframes(Lista *lista)
{
    if (lista == NULL || lista->primero == NULL)
    {
        printf("La lista ya esta vaciaa o no existe.\n");
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

Dataframe *load(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        establecer_color(ROJO);
        printf("Error al abrir el archivo\n");
        return NULL;
    }

    char buffer[MAX_STRING];
    int numColumnas = 0;

    // Contar el número de columnas leyendo la primera línea
    if (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char *token = strtok(buffer, ",\n");
        while (token != NULL)
        {
            numColumnas++;
            token = strtok(NULL, ",\n");
        }
    }

    // Reservar memoria para el Dataframe
    Dataframe *df = (Dataframe *)malloc(sizeof(Dataframe));
    df->numFilas = 0;
    df->numColumnas = numColumnas;
    df->columnas = (Columna *)malloc(numColumnas * sizeof(Columna));

    // Reservar memoria para los datos y los valores nulos de cada columna
    for (int i = 0; i < numColumnas; i++)
    {
        df->columnas[i].datos = (void *)malloc(MAX_FILAS * sizeof(char *)); // Almacenamos punteros a cadenas
        df->columnas[i].esNulo = (unsigned char *)malloc(MAX_FILAS * sizeof(unsigned char));
        df->columnas[i].numFilas = 0;
    }

    rewind(file); // Reiniciar el archivo

    // Leer nombres de columnas
    if (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        char *token = strtok(buffer, ",\n");
        int colIndex = 0;
        while (token != NULL)
        {
            strncpy(df->columnas[colIndex].nombre, token, 30);
            colIndex++;
            token = strtok(NULL, ",\n");
        }
    }

    // Leer filas restantes
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        int colIndex = 0;
        char *token = strtok(buffer, ",\n");
        while (token != NULL && colIndex < numColumnas)
        {
            // Detectar tipo
            TipoDato tipo = detectar_tipo(token);
            df->columnas[colIndex].tipo = tipo;

            // Manejar valores nulos
            if (df->columnas[colIndex].numFilas < MAX_FILAS)
            {
                if (token == NULL || strlen(token) == 0 || strspn(token, " \t\n\r") == strlen(token)) // Token vacío o solo espacios
                {
                    df->columnas[colIndex].esNulo[df->columnas[colIndex].numFilas] = 1;
                    ((char **)df->columnas[colIndex].datos)[df->columnas[colIndex].numFilas] = NULL;
                }
                else
                {
                    df->columnas[colIndex].esNulo[df->columnas[colIndex].numFilas] = 0;
                    ((char **)df->columnas[colIndex].datos)[df->columnas[colIndex].numFilas] = strdup(token);
                }

                df->columnas[colIndex].numFilas++;
            }

            colIndex++;
            token = strtok(NULL, ",\n");
        }
        df->numFilas++;
    }

    // Crear índice
    df->indice = (int *)malloc(df->numFilas * sizeof(int));
    for (int i = 0; i < df->numFilas; i++)
    {
        df->indice[i] = i;
    }

    // imprimirDataframe(df); // Imprimir para verificar
    fclose(file);
    return df;
}

void prompt(Lista *lista, int indice, int activo)
{
    activo = 1;
    if (lista == NULL || lista->primero == NULL || activo == -1)
    {
        printf("[?]:>");
        return;
    }
    Nodo *actual = lista->primero;
    int i = 0;

    while (actual != NULL && i < indice)
    {
        actual = actual->siguiente;
        i++;
    }

    if (actual == NULL || actual->df == NULL)
    {
        printf("[?]:>");
        return;
    }
    else
    {
        printf("[df%d: %d,%d]:> ", indice, actual->df->numFilas, actual->df->numColumnas);
    }
}

void prompt2(Dataframe *df)
{
    if (df == NULL)
    {
        printf("[?]:> ");
        return;
    }
    else
    {
        // Usar el nombre del DataFrame en el prompt
        printf("[%s: %d,%d]:> ", df->nombre, df->numFilas, df->numColumnas);
    }
}

void procesarComando(char *comando)
{
    char *token = strtok(comando, " ");
    if (token != NULL && strcmp(token, "load") == 0)
    {
        token = strtok(NULL, " ");
        printf(token);
    }
}
const char *obtener_nombre_tipo(TipoDato tipo)
{
    switch (tipo)
    {
    case NUMERICO:
        return "NUMERICO";
    case FECHA:
        return "FECHA";
    case TEXTO:
        return "TEXTO";
    default:
        return "DESCONOCIDO";
    }
}

void meta(Dataframe *df)
{
    if (df == NULL)
    {
        establecer_color(ROJO);
        printf("El dataframe es nulo\n");
        return;
    }

    for (int i = 0; i < df->numColumnas; i++)
    {
        establecer_color(VERDE);
        // Contar valores nulos en cada columna
        int numNulos = 0;
        for (int j = 0; j < df->columnas[i].numFilas; j++)
        {
            if (df->columnas[i].esNulo[j] == 1)
            {
                numNulos++;
            }
        }

        // Mostrar información de la columna incluyendo el número de valores nulos
        printf("Columna %d: %s tipo datos: %s numero filas: %d valores nulos: %d\n",
               i,
               df->columnas[i].nombre,
               obtener_nombre_tipo(df->columnas[i].tipo),
               df->columnas[i].numFilas,
               numNulos);
    }
}

void delcolumn(Dataframe *df, const char *nombreColumna)
{
    // comprobar si el dataframe es nulo
    if (df == NULL)
    {
        establecer_color(ROJO);
        printf("El dataframe es nulo\n");
        return;
    }

    // usamos indice para poder recorrer  las columnas del dataframe
    int indice = -1;
    for (int i = 0; i <= df->numColumnas; i++)
    { // bucle para recorrer las columnas
        if (strcmp(df->columnas[i].nombre, nombreColumna) == 0)
        {               // compara el nombre de la columna con el introducido por parametro
            indice = i; // guarda el indice de la columna de coindida con el nombre.
            break;
        }
    }

    if (indice == -1)
    {
        establecer_color(ROJO);
        printf("Columna no encontrada\n");
        return;
    }

    free(df->columnas[indice].datos);  // libera la memoria de los datos de la columna
    free(df->columnas[indice].esNulo); // libera la memoria de los datos nulos de la columna

    for (int i = indice; i < df->numColumnas - 1; i++)
    {
        df->columnas[i] = df->columnas[i + 1]; // mueve las columnas una posicion a la izquierda
    }

    df->numColumnas--;

    df->columnas = (Columna *)realloc(df->columnas, df->numColumnas * sizeof(Columna));
    if (df->numColumnas > 0 && df->columnas == NULL)
    {
        establecer_color(ROJO);
        printf("Error al reasignar memoria.\n");
        return;
    }
}

void view(Dataframe *df, int n)
{
    if (df == NULL)
    {
        establecer_color(ROJO);
        printf("El dataframe es nulo\n");
        return;
    }

    if (n == 0)
    {
        establecer_color(ROJO);
        printf("El valor de n es invalido\n");
        return;
    }

    // Ajustar n si es mayor al número de filas disponibles
    if (n > df->numFilas)
    {
        n = df->numFilas;
    }

    establecer_color(VERDE);

    // Imprimir encabezados
    for (int i = 0; i < df->numColumnas; i++)
    {
        printf("%-15s", df->columnas[i].nombre);
    }
    printf("\n");

    if (n > 0)
    {
        // Mostrar las primeras 'n' filas
        for (int fila = 0; fila < n; fila++)
        {
            int actual = df->indice[fila];
            for (int col = 0; col < df->numColumnas; col++)
            {
                char *valor = ((char **)df->columnas[col].datos)[actual];
                printf("%-15s", valor ? valor : "#N/A");
            }
            printf("\n");
        }
    }
    else
    {
        n = -n;

        if (n > df->numFilas)
        {
            n = df->numFilas;
        }
        for (int i = df->numFilas - 1; i >= df->numFilas - n; i--)
        {
            for (int j = 0; j < df->numColumnas; j++)
            {
                if (df->columnas[j].esNulo[i] == 1)
                {
                    printf("%-15s", "NULL");
                }
                else
                {
                    printf("%-15s", ((char **)df->columnas[j].datos)[i]);
                }
            }
            printf("\n");
        }
    }
}

void delnull(Dataframe *df, const char *columnaNombre)
{
    if (df == NULL || columnaNombre == NULL)
    {
        printf("El DataFrame o el nombre de la columna es nulo.\n");
        return;
    }

    // Buscar la columna indicada por nombre
    int colIndex = -1;
    for (int i = 0; i < df->numColumnas; i++)
    {
        if (strcmp(df->columnas[i].nombre, columnaNombre) == 0)
        {
            colIndex = i;
            break;
        }
    }
    if (colIndex == -1)
    {
        establecer_color(ROJO);
        printf("Columna no encontrada.\n");
        return;
    }

    int filasEliminadas = 0;

    // Recorremos las filas en orden inverso para evitar problemas con el desplazamiento de datos
    for (int i = df->numFilas - 1; i >= 0; i--) // bucle alreves
    {
        if (df->columnas[colIndex].esNulo[i] == 1) // Si el valor es nulo
        {
            // Liberar memoria de cada columna para la fila eliminada
            for (int j = 0; j < df->numColumnas; j++)
            {
                if (df->columnas[j].esNulo[i] == 0) // No es nulo
                {
                    free(((char **)df->columnas[j].datos)[i]);
                }
            }

            // Desplazar las filas hacia arriba para mantener el orden
            for (int j = i; j < df->numFilas - 1; j++)
            {
                for (int k = 0; k < df->numColumnas; k++)
                {
                    ((char **)df->columnas[k].datos)[j] = ((char **)df->columnas[k].datos)[j + 1];
                    df->columnas[k].esNulo[j] = df->columnas[k].esNulo[j + 1];
                }
            }

            filasEliminadas++;
            df->numFilas--; // Reducir el número de filas
        }
    }

    if (filasEliminadas > 0)
    {
        establecer_color(VERDE);
        printf("Se han eliminado %d filas.\n", filasEliminadas);
    }
    else
    {
        establecer_color(ROJO);
        printf("No se encontraron filas con valores nulos en la columna '%s'.\n", columnaNombre);
    }
}

void save(Dataframe *df, const char *nombreFichero)
{
    // Comprobar si hay un DataFrame activo
    if (df == NULL)
    {
        establecer_color(ROJO);
        printf("No hay un DataFrame activo.\n");
        return;
    }

    // Abrir el archivo para escribir
    FILE *archivo = fopen(nombreFichero, "w");
    if (archivo == NULL)
    {
        establecer_color(ROJO);
        printf("Error al abrir el archivo para guardar.\n");
        return;
    }

    // Imprimir los nombres de las columnas (cabecera del CSV)
    for (int i = 0; i < df->numColumnas; i++)
    {
        fprintf(archivo, "%s", df->columnas[i].nombre);
        if (i < df->numColumnas - 1)
        {
            fprintf(archivo, ","); // Separador de columnas
        }
    }
    fprintf(archivo, "\n"); // Salto de línea después de la cabecera

    // Imprimir los datos de cada fila
    for (int i = 0; i < df->numFilas; i++)
    {
        for (int j = 0; j < df->numColumnas; j++)
        {
            if (df->columnas[j].esNulo[i] == 1)
            {
                fprintf(archivo, " "); // Imprimir "NULL" para los valores nulos
            }
            else
            {
                // Si la columna es de tipo TEXTO
                if (df->columnas[j].tipo == TEXTO)
                {
                    fprintf(archivo, "%s", ((char **)df->columnas[j].datos)[i]);
                }
                // Si la columna es de tipo NUMERICO
                else if (df->columnas[j].tipo == NUMERICO)
                {
                    fprintf(archivo, "%s", ((char **)df->columnas[j].datos)[i]); // Asumiendo que los datos numéricos se almacenan como texto
                }
                // Si la columna es de tipo FECHA
                else if (df->columnas[j].tipo == FECHA)
                {
                    fprintf(archivo, "%s", ((char **)df->columnas[j].datos)[i]); // Asumiendo que las fechas están en formato de cadena
                }
            }

            // Agregar la coma entre columnas, excepto después de la última columna
            if (j < df->numColumnas - 1)
            {
                fprintf(archivo, ",");
            }
        }
        fprintf(archivo, "\n"); // Salto de línea después de cada fila
    }

    establecer_color(VERDE);
    printf("Archivo Guardado\n");
    // Cerrar el archivo

    fclose(archivo);
}

void quarter(Dataframe *df, const char *nombreColumna, const char *nombreNuevaColumna)
{
    // Comprobar si hay un DataFrame activo
    if (df == NULL)
    {
        establecer_color(ROJO);
        printf("No hay un DataFrame activo.\n");
        return;
    }

    // Comprobar si la columna indicada existe y es de tipo FECHA
    int colIndex = -1;
    for (int i = 0; i < df->numColumnas; i++)
    {
        if (strcmp(df->columnas[i].nombre, nombreColumna) == 0)
        {
            if (df->columnas[i].tipo == FECHA)
            {
                colIndex = i;
                break;
            }
            else
            {
                establecer_color(ROJO);
                printf("La columna %s no es de tipo FECHA.\n", nombreColumna);
                return;
            }
        }
    }

    // Si la columna no existe
    if (colIndex == -1)
    {
        establecer_color(ROJO);
        printf("La columna %s no existe.\n", nombreColumna);
        return;
    }

    // Comprobar si ya existe una columna con el mismo nombre para la nueva columna
    for (int i = 0; i < df->numColumnas; i++)
    {
        if (strcmp(df->columnas[i].nombre, nombreNuevaColumna) == 0)
        {
            establecer_color(ROJO);
            printf("Ya existe una columna con el nombre %s.\n", nombreNuevaColumna);
            return;
        }
    }

    // Aumentar el número de columnas para la nueva columna
    df->numColumnas++;
    df->columnas = realloc(df->columnas, df->numColumnas * sizeof(Columna));

    // Inicializar la nueva columna
    strcpy(df->columnas[df->numColumnas - 1].nombre, nombreNuevaColumna);
    df->columnas[df->numColumnas - 1].tipo = TEXTO;
    df->columnas[df->numColumnas - 1].numFilas = df->numFilas;
    df->columnas[df->numColumnas - 1].esNulo = malloc(df->numFilas * sizeof(unsigned char));
    df->columnas[df->numColumnas - 1].datos = malloc(df->numFilas * sizeof(char *));

    // Inicializar los valores de la nueva columna
    for (int i = 0; i < df->numFilas; i++)
    {
        // Si el valor de la fecha es nulo, asignar "#N/A"
        if (df->columnas[colIndex].esNulo[i] == 1)
        {
            df->columnas[df->numColumnas - 1].esNulo[i] = 1;
            ((char **)df->columnas[df->numColumnas - 1].datos)[i] = strdup("#N/A");
        }
        else
        {
            // Obtener la fecha de la columna existente
            char *fecha = ((char **)df->columnas[colIndex].datos)[i];
            int dia, mes, anio;

            // Intentar parsear la fecha con los formatos aceptados
            if (sscanf(fecha, "%4d-%2d-%2d", &anio, &mes, &dia) == 3 ||
                sscanf(fecha, "%4d/%2d/%2d", &anio, &mes, &dia) == 3)
            {
                // Determinar el trimestre en función del mes
                char *trimestre;
                if (mes >= 1 && mes <= 3)
                {
                    trimestre = "Q1";
                }
                else if (mes >= 4 && mes <= 6)
                {
                    trimestre = "Q2";
                }
                else if (mes >= 7 && mes <= 9)
                {
                    trimestre = "Q3";
                }
                else if (mes >= 10 && mes <= 12)
                {
                    trimestre = "Q4";
                }
                else
                {
                    trimestre = "#N/A"; // En caso de que el mes no sea válido
                }

                df->columnas[df->numColumnas - 1].esNulo[i] = 0;
                ((char **)df->columnas[df->numColumnas - 1].datos)[i] = strdup(trimestre);
            }
            else
            {
                df->columnas[df->numColumnas - 1].esNulo[i] = 1;
                ((char **)df->columnas[df->numColumnas - 1].datos)[i] = strdup("#N/A");
            }
        }
    }
 

    establecer_color(VERDE);
    printf("Columna %s creada con éxito.\n", nombreNuevaColumna);
}


void list(Lista *lista, Dataframe *df)
{
    // Verificar si la lista está vacía
    if (lista == NULL || lista->primero == NULL)
    {
        printf("La lista esta vacia.\n");
        return;
    }

    if (df == NULL)
    {
        printf("No se paso un DataFrame válido.\n");
        return;
    }

    int i = 1; // Contador para los DataFrames
    Nodo *actual = lista->primero;

    printf("Listado de DataFrames cargados en memoria:\n");

    // Recorrer todos los nodos de la lista
    while (actual != NULL)
    {
        if (actual->df != NULL)
        {
            // Acceder al nombre desde el DataFrame directamente
            printf("Nombre: %s, %d filas, %d columnas\n",
                   actual->df->nombre,
                   actual->df->numFilas,
                   actual->df->numColumnas);
            i++;
        }
        else
        {
            printf("Nodo sin DataFrame.\n");
        }
        actual = actual->siguiente; // Mover al siguiente nodo
    }

    // Acceso adicional directo al DataFrame proporcionado como parámetro
    // printf("Nombre: %s, %d filas, %d columnas\n", df->nombre, df->numFilas, df->numColumnas);

    printf("\nNumero total de DataFrames en la lista: %d\n", lista->numDFs);
}

void list2(Lista *lista)
{
    // Verificar si la lista está vacía
    if (lista == NULL || lista->primero == NULL)
    {
        printf("La lista esta vacia.\n");
        return;
    }

    int totalDFs = lista->numDFs;
    Nodo *actual = lista->primero;
    Nodo **nodos = malloc(totalDFs * sizeof(Nodo *));

    if (nodos == NULL)
    {
        printf("Error al asignar memoria para la lista invertida.\n");
        return;
    }

    // Guardar los nodos en un array
    int i = 0;
    while (actual != NULL)
    {
        nodos[i++] = actual;
        actual = actual->siguiente;
    }

    printf("Listado de DataFrames cargados en memoria (orden inverso):\n");

    // Imprimir en orden inverso
    for (int j = totalDFs - 1; j >= 0; j--)
    {
        Dataframe *df = nodos[j]->df;
        if (df != NULL)
        {
            establecer_color(VERDE);
            printf(" Nombre: %s, %d filas, %d columnas\n",
                   df->nombre, df->numFilas, df->numColumnas);
        }
        else
        {
            printf("Nodo sin DataFrame.\n");
        }
    }

    free(nodos);
    printf("\nNumero total de DataFrames en la lista: %d\n", totalDFs);
}
void filter_dataframe(Dataframe *df, const char *nombre_columna, const char *operador, const char *valor)
{
    if (df == NULL)
    {
        printf("mError: No hay un DataFrame activo.\n");
        return;
    }

    // Validar el operador
    const char *operadores_validos[] = {"eq", "neq", "gt", "lt"};
    int operador_valido = 0;
    for (int i = 0; i < sizeof(operadores_validos) / sizeof(operadores_validos[0]); i++)
    {
        if (strcmp(operador, operadores_validos[i]) == 0)
        {
            operador_valido = 1;
            break;
        }
    }

    if (!operador_valido)
    {
        printf("mError: Operador no valido. Los operadores valido son: eq, neq, gt, lt\n");
        return;
    }

    int columna = -1;

    // Buscar la columna por nombre
    for (int i = 0; i < df->numColumnas; i++)
    {
        if (strcmp(df->columnas[i].nombre, nombre_columna) == 0)
        {
            columna = i;
            break;
        }
    }

    if (columna == -1)
    {
        printf("Error: La columna %s no existe.\n", nombre_columna);
        return;
    }

    // Determinar el tipo de la columna
    TipoDato tipo = df->columnas[columna].tipo;

    // Iterar de abajo hacia arriba para evitar problemas de índices
    for (int i = df->numFilas - 1; i >= 0; i--)
    {
        if (df->columnas[columna].esNulo[i] == 1)
        {
            continue; // Ignorar valores nulos
        }

        char *dato = ((char **)df->columnas[columna].datos)[i];
        int cumple_condicion = 0;

        // Filtrar por tipo
        if (tipo == TEXTO)
        {
            // Operadores para texto
            if (strcmp(operador, "eq") == 0)
            {
                cumple_condicion = strcmp(dato, valor) == 0;
            }
            else if (strcmp(operador, "neq") == 0)
            {
                cumple_condicion = strcmp(dato, valor) != 0;
            }
        
        }
        else if (tipo == NUMERICO)
        {
            // Operadores para números
            double valor_fila = atof(dato);
            double valor_numerico = atof(valor);

            if (strcmp(operador, "eq") == 0)
            {
                cumple_condicion = valor_fila == valor_numerico;
            }
            else if (strcmp(operador, "neq") == 0)
            {
                cumple_condicion = valor_fila != valor_numerico;
            }
            else if (strcmp(operador, "gt") == 0)
            {
                cumple_condicion = valor_fila > valor_numerico;
            }
            else if (strcmp(operador, "lt") == 0)
            {
                cumple_condicion = valor_fila < valor_numerico;
            }
        }
        else if (tipo == FECHA)
        {
            // Operadores para fechas (formato YYYY/MM/DD)
            if (!fechaValida((char *)dato) || !fechaValida((char *)valor))
            {
                printf("mError: Fecha no válida en la comparacion.\n");
                return;
            }

            if (strcmp(operador, "eq") == 0)
            {
                cumple_condicion = strcmp(dato, valor) == 0;
            }
            else if (strcmp(operador, "neq") == 0)
            {
                cumple_condicion = strcmp(dato, valor) != 0;
            }
            else if (strcmp(operador, "gt") == 0)
            {
                cumple_condicion = strcmp(dato, valor) > 0;
            }
            else if (strcmp(operador, "lt") == 0)
            {
                cumple_condicion = strcmp(dato, valor) < 0;
            }
        }

        // Si no cumple la condición, eliminar la fila
        if (!cumple_condicion)
        {
            // Eliminar datos de cada columna
            for (int k = 0; k < df->numColumnas; k++)
            {
                free(((char **)df->columnas[k].datos)[i]);
            }

            // Desplazar los datos hacia arriba
            for (int j = i; j < df->numFilas - 1; j++)
            {
                df->indice[j] = df->indice[j + 1];
                for (int k = 0; k < df->numColumnas; k++)
                {
                    ((char **)df->columnas[k].datos)[j] = ((char **)df->columnas[k].datos)[j + 1];
                    df->columnas[k].esNulo[j] = df->columnas[k].esNulo[j + 1];
                }
            }

            df->numFilas--; // Reducir el conteo de filas
        }
    }

    // Imprimir el DataFrame actualizado
    imprimirDataframe(df);
}

void prefix(Dataframe *df, const char *nombre_columna, int n, const char *nombreNuevaColumna)
{
    // Comprobar si hay un DataFrame activo
    if (df == NULL)
    {
        printf("No hay un DataFrame activo.\n");
        return;
    }

    // Verificar que el número de caracteres 'n' es válido
    if (n <= 0)
    {
        printf("Error: El numero de caracteres debe ser mayor a 0.\n");
        return;
    }

    // Comprobar si la columna indicada existe y es de tipo TEXTO
    int colIndex = -1;
    for (int i = 0; i < df->numColumnas; i++)
    {
        if (strcmp(df->columnas[i].nombre, nombre_columna) == 0) // Comparación de nombre de columna
        {
            if (df->columnas[i].tipo == TEXTO)
            {
                colIndex = i;
                break;
            }
            else
            {
                printf("La columna %s no es de tipo TEXTO.\n", nombre_columna);
                return;
            }
        }
    }

    // Si la columna no existe
    if (colIndex == -1)
    {
        printf("La columna %s no existe.\n", nombre_columna);
        return;
    }

    // Comprobar si ya existe una columna con el nombre de la nueva columna
    for (int i = 0; i < df->numColumnas; i++)
    {
        if (strcmp(df->columnas[i].nombre, nombreNuevaColumna) == 0)
        {
            printf("Ya existe una columna con el nombre %s.\n", nombreNuevaColumna);
            return;
        }
    }

    // Aumentar el número de columnas para la nueva columna
    df->numColumnas++;
    df->columnas = realloc(df->columnas, df->numColumnas * sizeof(Columna));

    // Inicializar la nueva columna
    strcpy(df->columnas[df->numColumnas - 1].nombre, nombreNuevaColumna);
    df->columnas[df->numColumnas - 1].tipo = TEXTO;
    df->columnas[df->numColumnas - 1].numFilas = df->numFilas;
    df->columnas[df->numColumnas - 1].esNulo = malloc(df->numFilas * sizeof(unsigned char));
    df->columnas[df->numColumnas - 1].datos = malloc(df->numFilas * sizeof(char *));

    // Inicializar los valores de la nueva columna
    for (int i = 0; i < df->numFilas; i++)
    {
        // Si el valor de origen es nulo o su longitud es menor que 'n', asignar "#N/A"
        if (df->columnas[colIndex].esNulo[i] || strlen(((char **)df->columnas[colIndex].datos)[i]) < n)
        {
            df->columnas[df->numColumnas - 1].esNulo[i] = 1;
            ((char **)df->columnas[df->numColumnas - 1].datos)[i] = strdup("#N/A");
        }
        else
        {
            // Copiar el prefijo de longitud 'n' desde la columna origen
            char *datoOrigen = ((char **)df->columnas[colIndex].datos)[i];
            df->columnas[df->numColumnas - 1].esNulo[i] = 0;
            ((char **)df->columnas[df->numColumnas - 1].datos)[i] = malloc((n + 1) * sizeof(char));
            strncpy(((char **)df->columnas[df->numColumnas - 1].datos)[i], datoOrigen, n);
            ((char **)df->columnas[df->numColumnas - 1].datos)[i][n] = '\0';
        }
    }

    printf("Columna %s creada con exito.\n", nombreNuevaColumna);
}

void sort(Dataframe *df, const char *nombre_columna, const char *orden)
{
    if (df == NULL)
    {
        establecer_color(ROJO);
        printf("No hay un DataFrame activo.\n");
        return;
    }

    if (nombre_columna == NULL || (orden != NULL && strcmp(orden, "asc") != 0 && strcmp(orden, "des") != 0))
    {
        establecer_color(ROJO);
        printf("parametros mal escritos o incorrectos.\n");
        return;
    }

    int colIndex = -1;
    for (int i = 0; i < df->numColumnas; i++)
    {
        if (strcmp(df->columnas[i].nombre, nombre_columna) == 0)
        {
            colIndex = i;
            break;
        }
    }

    if (colIndex == -1)
    {
        establecer_color(ROJO);
        printf("La columna %s no existe.\n", nombre_columna);
        return;
    }

    int ascending = (orden == NULL || strcmp(orden, "asc") == 0) ? 1 : 0;

    for (int i = 0; i < df->numFilas - 1; i++)
    {
        for (int j = i + 1; j < df->numFilas; j++)
        {
            int isNull1 = df->columnas[colIndex].esNulo[i];
            int isNull2 = df->columnas[colIndex].esNulo[j];

            if (isNull1 || isNull2)
            {
                if (isNull1 && !isNull2 && ascending)
                {
                    // NULLs go first in ascending order
                    for (int k = 0; k < df->numColumnas; k++)
                    {
                        char **datos = (char **)df->columnas[k].datos;
                        char *temp = datos[i];
                        datos[i] = datos[j];
                        datos[j] = temp;

                        unsigned char tempNull = df->columnas[k].esNulo[i];
                        df->columnas[k].esNulo[i] = df->columnas[k].esNulo[j];
                        df->columnas[k].esNulo[j] = tempNull;
                    }
                }
                else if (!isNull1 && isNull2 && !ascending)
                {
                    // NULLs go last in descending order
                    for (int k = 0; k < df->numColumnas; k++)
                    {
                        char **datos = (char **)df->columnas[k].datos;
                        char *temp = datos[i];
                        datos[i] = datos[j];
                        datos[j] = temp;

                        unsigned char tempNull = df->columnas[k].esNulo[i];
                        df->columnas[k].esNulo[i] = df->columnas[k].esNulo[j];
                        df->columnas[k].esNulo[j] = tempNull;
                    }
                }
                continue;
            }

            char *valor1 = ((char **)df->columnas[colIndex].datos)[i];
            char *valor2 = ((char **)df->columnas[colIndex].datos)[j];

            int swap = 0;

            if (df->columnas[colIndex].tipo == NUMERICO)
            {
                double num1 = atof(valor1);
                double num2 = atof(valor2);

                // printf("num1: %s, num2: %s\n", valor1, valor2);
                // printf("num1: %f, num2: %f\n", num1, num2);

                if ((ascending && num1 > num2) || (!ascending && num1 < num2))
                {
                    swap = 1;
                }
            }
            else
            {
                int comparison = strcmp(valor1, valor2);
                if ((ascending && comparison > 0) || (!ascending && comparison < 0))
                {
                    swap = 1;
                }
            }

            if (swap)
            {
                for (int k = 0; k < df->numColumnas; k++)
                {
                    char **datos = (char **)df->columnas[k].datos;
                    char *temp = datos[i];
                    datos[i] = datos[j];
                    datos[j] = temp;

                    unsigned char tempNull = df->columnas[k].esNulo[i];
                    df->columnas[k].esNulo[i] = df->columnas[k].esNulo[j];
                    df->columnas[k].esNulo[j] = tempNull;
                }
            }
        }
    }

    establecer_color(VERDE);
    printf("Las filas se han ordenado correctamente por la columna %s en orden %s.\n",
           nombre_columna, ascending ? "ascendente" : "descendente");
}

int numeroDataframes(Lista *lista)
{
    if (lista == NULL)
    {
        return 0;
    }
    else
    {
        printf("Numero de Dataframes: %d\n", lista->numDFs);
    }
    return lista->numDFs;
}

void defaultName(Dataframe *df, int numeroDataframes)
{
    char nombre[40];
    sprintf(nombre, "df%d", numeroDataframes);
    strcpy(df->nombre, nombre);
}

void name(Lista *lista, Dataframe *df, const char *nombre)
{
    if (df == NULL)
    {
        printf("No hay un DataFrame activo.\n");
        return;
    }

    // Verificar si ya existe un DataFrame con el mismo nombre
    Nodo *actual = lista->primero;
    while (actual != NULL)
    {
        if (strcmp(actual->df->nombre, nombre) == 0)
        {
            // Si el nombre ya existe, mostramos un error
            establecer_color(ROJO);
            printf("Error: Ya existe un DataFrame con el nombre '%s'. No se puede asignar ese nombre.\n", nombre);
            return;
        }
        actual = actual->siguiente;
    }

    // Si no existe un DataFrame con el mismo nombre, asignamos el nuevo nombre
    strcpy(df->nombre, nombre);
    printf("Nombre asignado: %s\n", df->nombre);
}
void setActiveDataFrame(Lista *lista, const char *nombre, Dataframe **activo)
{
    // Verificar que el nombre es válido (sin parámetros adicionales)
    if (strchr(nombre, ' ') != NULL)
    {
        establecer_color(ROJO);
        printf("Error: El nombre del DataFrame no debe contener parametros adicionales.\n");
        return;
    }

    // Verificar si el DataFrame existe en la lista
    Nodo *actual = lista->primero;
    int encontrado = 0;

    while (actual != NULL)
    {
        if (strcmp(actual->df->nombre, nombre) == 0)
        {
            // Si encontramos el DataFrame, lo establecemos como activo
            *activo = actual->df; // Modificamos el puntero activo
            printf("El DataFrame %s es ahora el DataFrame activo.\n", nombre);
            encontrado = 1;
            break;
        }
        actual = actual->siguiente;
    }

    // Si no encontramos el DataFrame, mostramos un error
    if (!encontrado)
    {
        establecer_color(ROJO);
        printf("Error: El DataFrame %s no existe.\n", nombre);
    }
}

void calcularMedia(Dataframe *df, const char *nombre_columna)
{
    if (df == NULL)
    {
        establecer_color(ROJO);
        printf("Error: No hay un DataFrame activo.\n");
        return;
    }

    int columna = -1;

    // Buscar la columna por nombre
    for (int i = 0; i < df->numColumnas; i++)
    {
        if (strcmp(df->columnas[i].nombre, nombre_columna) == 0)
        {
            columna = i;
            break;
        }
    }

    if (columna == -1)
    {
        establecer_color(ROJO);
        printf("Error: La columna '%s' no existe\n", nombre_columna);
        return;
    }

    // Verificar que la columna sea numérica
    if (df->columnas[columna].tipo != NUMERICO)
    {
        establecer_color(ROJO);
        printf("Error: La columna '%s' no es de tipo numérico.\n", nombre_columna);
        return;
    }

    double suma = 0.0;
    int count = 0;

    // Iterar por las filas para calcular la suma de valores no nulos
    for (int i = 0; i < df->numFilas; i++)
    {
        if (df->columnas[columna].esNulo[i] == 1)
        {
            continue; // Ignorar valores nulos
        }

        char *dato = ((char **)df->columnas[columna].datos)[i];
        double valor = atof(dato); // Convertir el dato a double
        suma += valor;
        count++;
    }

    if (count == 0)
    {
        establecer_color(ROJO);
        printf("Advertencia: Todos los valores en la columna '%s' son nulos.\n", nombre_columna);
        return;
    }

    double media = suma / count;

    // Imprimir el resultado
    printf("La columna '%s' tiene una media de: %.2f.\n", nombre_columna, media);
}

void renombrarColumna(Dataframe *df, const char *nombreActual, const char *nuevoNombre)
{
    if (df == NULL)
    {
        establecer_color(ROJO);
        printf("Error: No hay un DataFrame activo.\n");
        return;
    }

    
    if (nombreActual == NULL || nuevoNombre == NULL)
    {
        establecer_color(ROJO);
        printf("Error: Se deben proporcionar ambos nombres.\n");
        return;
    }

    // Buscar la columna por su nombre actual
    int columnaEncontrada = -1;
    for (int i = 0; i < df->numColumnas; i++)
    {
        if (strcmp(df->columnas[i].nombre, nombreActual) == 0)
        {
            columnaEncontrada = i;
            break;
        }
    }

    if (columnaEncontrada == -1)
    {
        establecer_color(ROJO);
        printf("Error: La columna '%s' no existe.\n", nombreActual);
        return;
    }

    // Renombrar la columna
    strncpy(df->columnas[columnaEncontrada].nombre, nuevoNombre, sizeof(df->columnas[columnaEncontrada].nombre) - 1);
    df->columnas[columnaEncontrada].nombre[sizeof(df->columnas[columnaEncontrada].nombre) - 1] = '\0'; // Asegurar el nulo final

    establecer_color(VERDE);
    printf("La columna '%s' ha sido renombrada a '%s'.\n", nombreActual, nuevoNombre);
}

void eliminarDataframe2(Lista *lista, const char *nombre)
{
    if (lista == NULL || lista->primero == NULL)
    {
        printf("\033[31mError: No hay DataFrames en la lista.\033[0m\n");
        return;
    }

    Nodo *actual = lista->primero;
    Nodo *anterior = NULL;

    while (actual != NULL)
    {
        if (strcmp(actual->df->nombre, nombre) == 0)
        {
            // Liberar memoria del DataFrame
            Dataframe *df = actual->df;

            for (int i = 0; i < df->numColumnas; i++)
            {
                // Liberar cada celda de datos de la columna
                for (int j = 0; j < df->numFilas; j++)
                {
                    free(((char **)df->columnas[i].datos)[j]);
                }

                // Liberar los arrays de datos y nulos de la columna
                free(df->columnas[i].datos);
                free(df->columnas[i].esNulo);
            }

            // Liberar el array de columnas y el índice del DataFrame
            free(df->columnas);
            free(df->indice);

            // Liberar la estructura del DataFrame
            free(df);

            // Eliminar el nodo de la lista
            if (anterior == NULL)
            {
                // Es el primer nodo
                lista->primero = actual->siguiente;
            }
            else
            {
                // Nodo intermedio o final
                anterior->siguiente = actual->siguiente;
            }

            // Liberar el nodo de la lista
            free(actual);

            // Decrementar el número de DataFrames
            lista->numDFs--;

            printf("\033[32mDataFrame '%s' eliminado con éxito.\033[0m\n", nombre);
            return;
        }

        anterior = actual;
        actual = actual->siguiente;
    }

    // Si no se encontró el DataFrame
    printf("\033[31mError: No se encontró un DataFrame con el nombre '%s'.\033[0m\n", nombre);
}