
#include <stddef.h> // Para NULL
// Tipo enumerado para representar los diferentes tipos de datos en las columnas
typedef enum
{
    TEXTO,
    NUMERICO,
    FECHA
} TipoDato;

// Estructura para representar una columna del dataframe
typedef struct
{
    char nombre[30];       // Nombre de la columna
    TipoDato tipo;         // Tipo de datos de la columna (TEXTO, NUMERICO, FECHA)
    void *datos;           // Puntero genérico para almacenar los datos de la columna
    unsigned char *esNulo; // Array paralelo, indica valores nulos (1/0: nulo/noNulo)
    int numFilas;          // Número de filas en la columna
} Columna;

// Estructura para representar el dataframe como un conjunto de columnas
typedef struct
{
    Columna *columnas; // Array de columnas (con tipos de datos distintos)
    int numColumnas;   // Número de columnas en el dataframe
    int numFilas;      // Número de filas (igual para todas las columnas)
    int *indice;       // Array para ordenar las filas
} Dataframe;

// Alias para tipos FECHA: 'Fecha' alias de 'struct tm' (#include <time.h>)

typedef struct tm Fecha;

// Estructura para representar un nodo de la lista
typedef struct NodoLista
{
    Dataframe *df;               // Puntero a un dataframe
    struct NodoLista *siguiente; // Puntero al siguiente nodo de la lista
} Nodo;

// Estructura para representar la lista de Dataframe’s
typedef struct
{
    int numDFs;    // Número de dataframes almacenados en la lista
    Nodo *primero; // Puntero al primer Nodo de la lista
} Lista;

void establecer_color(int color);
TipoDato detectar_tipo(char *valor);
Columna *crearColumna(char nombre[30], TipoDato tipo, void *datos, unsigned char *esNulo, int numFilas);
int esNumero(char *valor);
void eliminarColumna(Columna **columna);
void imprimirColumna(Columna *columna);
Dataframe *crearDataframe(int numColumnas, int numFilas);
void imprimirDataframe(Dataframe *df);
void eliminarDataframe(Dataframe **df);
void inicializarLista(Lista *lista);
void insertarDataframeLista(Lista *lista, Dataframe *df);
void imprimirLista(Lista *lista);
void eliminarTodosLosDataframes(Lista *lista);
void eliminarDataframeLista(Lista *lista, Dataframe *df);
void eliminarDataframes(Lista *lista);
Dataframe *load(char *filename);
void prompt(Lista *lista, int indice, int activo);

void procesarComando(char *comando);


