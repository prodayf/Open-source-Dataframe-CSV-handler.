# Dataframe Manager - README

## Overview

The **Dataframe Manager** is a command-line interface (CLI) program written in C that allows users to manage and manipulate structured data in the form of dataframes, similar to Python's pandas dataframes. The program supports loading data from CSV files, performing basic operations on the data, and saving the results back to CSV files. The dataframe can handle different types of data, including text, numeric, and date values, and can detect and handle null or anomalous values.

This document provides a detailed explanation of the program's functionality, including examples of how to use each command and how the operations work.

---

## Table of Contents

1. [Program Flow](#program-flow)
2. [Dataframe Structure](#dataframe-structure)
3. [Supported Data Types](#supported-data-types)
4. [Commands](#commands)
   - [quit](#quit)
   - [load](#load)
   - [meta](#meta)
   - [view](#view)
   - [sort](#sort)
   - [save](#save)
   - [filter](#filter)
   - [delnull](#delnull)
   - [delcolum](#delcolum)
   - [quarter](#quarter)
   - [df0 / df1 / df2 / ...](#df0--df1--df2--)
5. [Examples](#examples)
6. [Memory Management](#memory-management)
7. [Additional Features](#additional-features)
   - [Exercise 1: Modified `view` Command](#exercise-1-modified-view-command)
   - [Exercise 2: `name` Command](#exercise-2-name-command)
   - [Exercise 3: `list` Command](#exercise-3-list-command)
   - [Exercise 4: `prefix` Command](#exercise-4-prefix-command)

---

## Program Flow

The program follows a specific flow of control:

1. **Display Student Information**: The program starts by displaying the student's name, surname, and email.
2. **Show Prompt**: The program displays a prompt in white, waiting for user input.
3. **Read Command**: The user enters a command via the keyboard.
4. **Analyze Command**:
   - If the command is invalid, an error message is displayed in red, and the program returns to step 2.
   - If the command is empty, the program does nothing and returns to step 2.
   - If the command is `quit`, the program frees all dynamically allocated memory and terminates with a green "Fin..." message.
   - If the command is valid, the corresponding operation is executed, and the result (if any) is displayed in green. The program then returns to step 2.

---

## Dataframe Structure

The dataframe is implemented using the following structures:

- **`TipoDato`**: An enumerated type representing the different data types supported by the dataframe (`TEXTO`, `NUMERICO`, `FECHA`).
- **`Columna`**: Represents a column in the dataframe, containing:
  - `nombre`: The name of the column.
  - `tipo`: The data type of the column.
  - `datos`: A generic pointer to the data array.
  - `esNulo`: An array indicating whether each value in the column is null.
  - `numFilas`: The number of rows in the column.
- **`Dataframe`**: Represents the entire dataframe, containing:
  - `columnas`: An array of `Columna` structures.
  - `numColumnas`: The number of columns in the dataframe.
  - `numFilas`: The number of rows in the dataframe.
  - `indice`: An array used to sort the rows of the dataframe.

---

## Supported Data Types

The dataframe supports the following data types:

1. **Text**: Strings of characters.
2. **Numeric**: Integers and decimals.
3. **Date**: Dates in the format `YYYY-MM-DD`.

---

## Commands

### `quit`

- **Description**: Terminates the program, freeing all dynamically allocated memory.
- **Usage**: `quit`
- **Output**: Displays "Fin..." in green and exits the program.

### `load <nombre_fichero>`

- **Description**: Loads a CSV file into a new dataframe. The first row of the CSV file should contain the column names, and the second row should contain the data types.
- **Usage**: `load data.csv`
- **Output**: If successful, the loaded dataframe becomes the active dataframe, and the prompt changes to reflect the new dataframe's name and dimensions. If an error occurs, an error message is displayed in red.

### `meta`

- **Description**: Displays metadata about the active dataframe, including column names, data types, and the number of null values in each column.
- **Usage**: `meta`
- **Output**: Displays the metadata in green. If no dataframe is active, an error message is displayed in red.

### `view [n]`

- **Description**: Displays the first `n` rows of the active dataframe. If `n` is not specified, the default is 10 rows.
- **Usage**: `view 5` or `view`
- **Output**: Displays the rows in green. If `n` is negative, the last `n` rows are displayed in reverse order. Null values are represented as `#N/A`.

### `sort <nombre_columna> [asc/des]`

- **Description**: Sorts the rows of the active dataframe based on the specified column. The sorting can be ascending (`asc`) or descending (`des`). If no order is specified, the default is ascending.
- **Usage**: `sort age des`
- **Output**: The dataframe is sorted, and the prompt is updated. If an error occurs, an error message is displayed in red.

### `save <nombre_fichero>`

- **Description**: Saves the active dataframe to a CSV file.
- **Usage**: `save output.csv`
- **Output**: The dataframe is saved to the specified file. If no dataframe is active, an error message is displayed in red.

### `filter <nombre_columna> eq/neq/gt/lt <valor>`

- **Description**: Filters the active dataframe based on a condition applied to a specific column. The condition can be:
  - `eq`: Equal to
  - `neq`: Not equal to
  - `gt`: Greater than
  - `lt`: Less than
- **Usage**: `filter age gt 30`
- **Output**: The dataframe is filtered, and the prompt is updated. If an error occurs, an error message is displayed in red.

### `delnull <nombre_columna>`

- **Description**: Removes all rows from the active dataframe that contain null values in the specified column.
- **Usage**: `delnull age`
- **Output**: Displays the number of rows deleted in green. If an error occurs, an error message is displayed in red.

### `delcolum <nombre_columna>`

- **Description**: Deletes the specified column from the active dataframe.
- **Usage**: `delcolum age`
- **Output**: The column is deleted, and the prompt is updated. If an error occurs, an error message is displayed in red.

### `quarter <nombre_columna> <nombre_nueva_columna>`

- **Description**: Creates a new column in the active dataframe that indicates the quarter (`Q1`, `Q2`, `Q3`, `Q4`) of the year for each date in the specified column.
- **Usage**: `quarter date quarter`
- **Output**: The new column is added to the dataframe, and the prompt is updated. If an error occurs, an error message is displayed in red.

### `df0 / df1 / df2 / ...`

- **Description**: Switches the active dataframe to the specified dataframe.
- **Usage**: `df1`
- **Output**: The active dataframe is changed, and the prompt is updated. If the specified dataframe does not exist, an error message is displayed in red.

---

## Examples

### Example 1: Loading a CSV File

```markdown
[?]:> load data.csv
[df0: 100,5]:>
```

- The CSV file `data.csv` is loaded into a new dataframe named `df0`, which has 100 rows and 5 columns.

### Example 2: Viewing the First 5 Rows

```markdown
[df0: 100,5]:> view 5
| Name   | Age | Date       |
|--------|-----|------------|
| Alice  | 25  | 1998-05-12 |
| Bob    | 30  | 1993-08-22 |
| Charlie| #N/A| 1990-11-15 |
| David  | 40  | 1983-04-30 |
| Eve    | 35  | 1988-07-19 |
```

- The first 5 rows of the active dataframe are displayed. Null values are shown as `#N/A`.

### Example 3: Sorting by Age in Descending Order

```markdown
[df0: 100,5]:> sort age des
[df0: 100,5]:> view 5
| Name   | Age | Date       |
|--------|-----|------------|
| David  | 40  | 1983-04-30 |
| Eve    | 35  | 1988-07-19 |
| Bob    | 30  | 1993-08-22 |
| Alice  | 25  | 1998-05-12 |
| Charlie| #N/A| 1990-11-15 |
```

- The dataframe is sorted by the `Age` column in descending order.

---

## Memory Management

The program ensures proper memory management by dynamically allocating and freeing memory for dataframes, columns, and other structures. Functions are provided to create, delete, and manage dataframes and columns, ensuring that no memory leaks occur.

---

## Additional Features

### Exercise 1: Modified `view` Command

- **Description**: The `view` command now supports negative values for `n`, displaying the last `n` rows in reverse order.
- **Usage**: `view -5`
- **Output**: Displays the last 5 rows in reverse order.

### Exercise 2: `name` Command

- **Description**: Allows the user to assign a custom name to the active dataframe.
- **Usage**: `name my_dataframe`
- **Output**: The dataframe is renamed, and the prompt is updated to reflect the new name.

### Exercise 3: `list` Command

- **Description**: Lists all dataframes currently loaded in memory, along with their names, number of rows, and number of columns.
- **Usage**: `list`
- **Output**:
  ```markdown
  df0: 100 filas, 5 columnas
  my_dataframe: 50 filas, 3 columnas
  ```

### Exercise 4: `prefix` Command

- **Description**: Creates a new column containing the first `n` characters of the values in a specified text column.
- **Usage**: `prefix Name 3 ShortName`
- **Output**: A new column `ShortName` is added, containing the first 3 characters of each value in the `Name` column.

---

## Conclusion

The **Dataframe Manager** is a powerful tool for managing structured data in a command-line environment. With support for various data types, operations, and memory management, it provides a robust solution for handling dataframes in C.
