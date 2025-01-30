Dataframe Manager - C Program
Overview
This program is a Dataframe Manager implemented in C, designed to handle structured datasets similarly to Python's DataFrame. It provides basic data manipulation operations such as loading CSV files, filtering rows, sorting columns, and generating new columns, all through a Command Line Interface (CLI). The program supports different data types like Text, Numerical, and Date values and allows for error handling, such as detecting incorrect types and missing values.

Features
Load CSV files into DataFrames
Handle multiple DataFrames in memory
Provide basic operations like:
Viewing the first n rows of the DataFrame
Sorting data by columns
Filtering data based on conditions
Deleting rows or columns
Generating new columns derived from existing ones
Error handling for invalid commands and data inconsistencies (such as null values or incorrect data types)
Display the active DataFrame metadata
Save the current DataFrame to a CSV file
Supports a variety of commands to manipulate and interact with the data
Data Structure
The program uses the following structures to store and manipulate data:

c
Copiar
Editar
// Enum for data types
typedef enum {
    TEXTO,
    NUMERICO,
    FECHA
} TipoDato;

// Column structure for storing data in DataFrame
typedef struct {
    char nombre[30];       // Column name
    TipoDato tipo;         // Data type of the column (TEXTO, NUMERICO, FECHA)
    void *datos;           // Generic pointer to store column data
    unsigned char *esNulo; // Null value indicator (1 for null, 0 for not null)
    int numFilas;          // Number of rows in the column
} Columna;

// DataFrame structure containing multiple columns
typedef struct {
    Columna *columnas;     // Array of columns (with different data types)
    int numColumnas;       // Number of columns in the DataFrame
    int numFilas;          // Number of rows (same for all columns)
    int *indice;           // Array to sort rows
} Dataframe;

// Date structure using 'struct tm' from <time.h>
typedef struct tm Fecha;
Data Types Supported:
Text: Stored as a string of characters.
Numerical: Includes both integers and floating-point numbers.
Date: Stored in the format YYYY-MM-DD.
DataFrames List:
The program supports managing multiple DataFrames, stored in a linked list for dynamic access:

c
Copiar
Editar
// Linked list node for DataFrames
typedef struct NodoLista {
    Dataframe *df;               // Pointer to a DataFrame
    struct NodoLista *siguiente; // Pointer to the next node in the list
} Nodo;

// List of DataFrames
typedef struct {
    int numDFs;     // Number of DataFrames in the list
    Nodo *primero;  // Pointer to the first node in the list
} Lista;
Command Line Interface (CLI)
The program interacts with the user through the command line. The prompt initially shows as:

markdown
Copiar
Editar
[?]:>
Once a DataFrame is loaded, the prompt will change to display information about the active DataFrame:

markdown
Copiar
Editar
[df0: 100,5]:>
Here, df0 refers to the first DataFrame, and the numbers represent the number of rows and columns.

Commands
The following commands are available for interacting with the DataFrames:

quit

Exits the program and frees all dynamic memory.
Displays "EXIT PROGRAM" in green and returns to the terminal.
load <filename>

Loads a CSV file into the program as a new DataFrame.
Automatically becomes the active DataFrame.
Displays errors if data types or null values are incorrect.
meta

Displays metadata for the active DataFrame (column names, types, and null counts).
If no DataFrame is active, displays an error.
view [n]

Displays the first n rows of the active DataFrame (default is 10 rows).
If there are fewer rows, it displays all rows.
Null values are displayed as #N/A.
sort <column_name> [asc/des]

Sorts the active DataFrame by the specified column.
asc for ascending order (default), des for descending order.
save <filename>

Saves the active DataFrame to a CSV file.
The first row contains the column names.
filter <column_name> eq/neq/gt/lt <value>

Filters rows based on conditions (e.g., "equal", "not equal", "greater than", "less than").
The column and value must match in type.
delnull <column_name>

Deletes all rows containing null values in the specified column.
Displays the number of rows deleted.
delcolum <column_name>

Deletes the specified column from the DataFrame.
quarter <column_name> <new_column_name>

Creates a new column indicating the fiscal quarter for each date in the specified column (Q1, Q2, Q3, Q4, or #N/A for null values).
df0 / df1 / df2 / ...

Switches the active DataFrame to the specified one.
If the DataFrame doesn't exist, an error is shown.
Requirements
C Language: The program is implemented in C and uses the standard library (stdio.h, stdlib.h, string.h, time.h).
Memory Management: The program must efficiently manage memory using dynamic allocation and free operations.
Error Handling: Invalid commands and data inconsistencies should be reported with appropriate error messages.
Example
markdown
Copiar
Editar
[?]:> load data.csv
[df0: 100,5]:> meta
Column 1: TEXTO, Nulls: 2
Column 2: NUMERICO, Nulls: 0
...
[df0: 100,5]:> filter Column1 eq 'John'
[df0: 50,5]:> sort Column2 des
[df0: 50,5]:> save filtered_data.csv
[df0: 50,5]:> quit
EXIT PROGRAM
Compilation and Execution
To compile and run the program, use the following commands:

bash
Copiar
Editar
gcc -o dataframe_manager dataframe_manager.c
./dataframe_manager
Conclusion
This C program offers a command-line interface for managing DataFrames, similar to Python's Pandas library, providing essential data manipulation operations like sorting, filtering, and saving data. The program handles different data types and includes extensive error checking for invalid data or commands.
