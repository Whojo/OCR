//
//  Matrix.c
//  OCR
//
//  Created by Moustapha Diop on 14/09/2019.
//  Copyright © 2019 Moustapha Diop. All rights reserved.
//

#include "Matrix.h"

// Les differentes erreures possibles
#define ErrorParameter(function) \
printf("Error Parameter : Entres are negative : This error comes from -> %s \n", function); \
exit(0);

#define ErrorMemory(memory) \
printf("Error Memory : Cannot access to this memory : This error comes from -> %s \n", memory); \
exit(0);

#define ErrorSizeAddition(rowA, columnA, rowB, columnB, function) \
printf("Error Size Addition : Wrong size for addition %d x %d + %d x %d : This error comes from -> %s \n", rowA, columnA, rowB, columnB, function); \
exit(0);

#define ErrorSizeMultiplication(rowA, columnA, rowB, columnB, function) \
printf("Error Size Multiplication : Wrong size for multiplication %d x %d + %d x %d : This error comes from -> %s \n", rowA, columnA, rowB, columnB, function); \
exit(0);

#define ErrorGetColumns(start, end, function) \
printf("Error Get Columns : Cannot get this columns from %d to %d : This error comes from -> %s \n", start, end, function); \
exit(0);

#define ErrorGetRows(start, end, function) \
printf("Error Get Rows : Cannot get this rows from %d to %d : This error comes from -> %s \n", start, end, function); \
exit(0);

#define ErrorSizeReplaceValues(rowA, columnA, rowB, columnB, function) \
printf("Error Size Replace Values : Wrong size for replacement %d x %d + %d x %d : This error comes from -> %s \n", rowA, columnA, rowB, columnB, function); \
exit(0);


/*Return a matrix rows by columns with 0*/
Matrix *constructeurMatrix(int rows, int columns){
    if (rows < 1 || columns < 1){
        ErrorParameter("Constructeur Matrix");
    }
    
    Matrix *matrix = malloc(sizeof(Matrix));
    matrix->columns = columns;
    matrix->rows = rows;
    matrix->matrice = malloc(sizeof(double) * (rows));
    
    // Check if we have access to this memory
    if (matrix->matrice == NULL){
        ErrorMemory("Constructeur Matrix");
    }
    
    for(int i = 0; i < rows; i++){
        //matrix->matrice[i] = calloc(sizeof(double), columns);
        matrix->matrice[i] = malloc(sizeof(double) * columns);
        
        // Check if we have access to this memory
        if (matrix->matrice[i] == NULL){
            ErrorMemory("Constructeur Matrix");
        }
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix->matrice[i][j] = 0;
        }
    }
    
    return matrix;
}


/*Print the matrix in parameter*/
void printMatrix(Matrix *matrix){
    if (matrix == NULL){
        ErrorMemory("Print Matrix")
    }
    for(unsigned int i = 0; i < matrix->rows; i++){
        for(unsigned int j = 0; j < matrix->columns; j++){
            printf("%f ", matrix->matrice[i][j]);
        }
        printf("\n");
    }
}


/*Create a personalize matrix*/
Matrix *createMatrix(int row, int columns, double listeMatrix[]){
    if (row < 1 || columns < 1){
        ErrorParameter("Create Matrix")
    }
    
    Matrix *matrix = constructeurMatrix(row, columns);
    unsigned int lenListeMatrix = 0;
    
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < columns; j++, lenListeMatrix++) {
            matrix->matrice[i][j] = listeMatrix[lenListeMatrix];
        }
    }
    
    return matrix;
}


/*Free memory by destroying the matrix*/
void destroyMatrix(Matrix *matrix){
    if (matrix == NULL){
        ErrorMemory("Destroy Matrix")
    }
    
    if (matrix->matrice != NULL){
        for(unsigned i = 0; i < matrix->rows; i++){
            if (matrix->matrice[i] != NULL){
                free(matrix->matrice[i]);
            }
        }
        
        free(matrix->matrice);
    }
    
    if (matrix != NULL){
        free(matrix);
    }    
}


/*Multiplication of the matrix by a double*/
Matrix *scalarMultiplyMatrix(Matrix *matrix, double scalar){
    if (matrix == NULL){
        ErrorMemory("Scalar Multiply Matrix")
    }
    
    for(unsigned int i = 0; i < matrix->rows; i++){
        for (unsigned int j = 0; j < matrix->columns; j++) {
            matrix->matrice[i][j] *= scalar;
        }
    }
    
    return matrix;
}


/*Addition of the matrix by a double*/
Matrix *scalarAdditionMatrix(Matrix *matrix, double scalar){
    if (matrix == NULL){
        ErrorMemory("Scalar Addition Matrix")
    }
    
    for(unsigned int i = 0; i < matrix->rows; i++){
        for(unsigned int j = 0; j < matrix->columns; j++){
            matrix->matrice[i][j] += scalar;
        }
    }
    
    return matrix;
}


/*Addition of two matrices*/
Matrix *AdditionMatrix(Matrix *matrixA, Matrix *matrixB){
    if (matrixA == NULL || matrixB == NULL){
        ErrorMemory("Addition Matrix")
    }
    
    if (matrixA->rows != matrixB->rows || matrixA->columns != matrixB->columns){
        ErrorSizeAddition(matrixA->rows, matrixA->columns, matrixB->rows, matrixB->columns, "Addition Matrix")
    }
    
    Matrix *sumMatrix = constructeurMatrix(matrixB->rows, matrixB->columns);
    
    for (unsigned int i = 0; i < matrixA->rows; i++){
        for(unsigned int j = 0; j < matrixA->columns; j++){
            sumMatrix->matrice[i][j] = matrixA->matrice[i][j] + matrixB->matrice[i][j];
        }
    }
    
    return sumMatrix;
}


/*Substraction of two matrices*/
Matrix *subtractMatrix(Matrix *matrixA, Matrix *matrixB){
    if (matrixA == NULL || matrixB == NULL){
        ErrorMemory("Substract Matrix")
    }
    
    if (matrixA->rows != matrixB->rows || matrixA->columns != matrixB->columns){
        ErrorSizeAddition(matrixA->rows, matrixA->columns, matrixB->rows, matrixB->columns, "Substract Matrix")
    }
    
    Matrix *sumMatrix = constructeurMatrix(matrixB->rows, matrixB->columns);
    
    for (unsigned int i = 0; i < matrixA->rows; i++){
        for(unsigned int j = 0; j < matrixA->columns; j++){
            sumMatrix->matrice[i][j] = matrixA->matrice[i][j] - matrixB->matrice[i][j];
        }
    }
    
    return sumMatrix;
}


/*Multiplication value by value of the matrices*/
Matrix *MultiplyElementsMatrix(Matrix *matrixA, Matrix *matrixB){
    if (matrixA == NULL || matrixB == NULL){
        ErrorMemory("Multiply Elements Matrix")
    }
    
    if (matrixB->rows != matrixA->rows || matrixB->columns != matrixA->columns){
        ErrorSizeMultiplication(matrixA->rows, matrixA->columns, matrixB->rows, matrixB->columns, "Multiply Elements Matrix")
    }
    
    Matrix *multiplyMatrix = constructeurMatrix(matrixA->rows, matrixA->columns);
    
    for (unsigned int i = 0; i < matrixB->rows; i++) {
        for (unsigned int j = 0; j < matrixB->columns; j++) {
            multiplyMatrix->matrice[i][j] = matrixB->matrice[i][j] * matrixA->matrice[i][j];
        }
    }
    
    return multiplyMatrix;
}


/*Multiplication of two matrices*/
Matrix *MultiplyMatrix(Matrix *matrixA, Matrix *matrixB){
    if (matrixA == NULL || matrixB == NULL){
        ErrorMemory("Multiply Matrix")
    }
    
    if (matrixA->columns != matrixB->rows){
        ErrorSizeMultiplication(matrixA->rows, matrixA->columns, matrixB->rows, matrixB->columns, "Multiply Matrix")
    }
    
    Matrix *multiplyMatrix = constructeurMatrix(matrixA->rows, matrixB->columns);
    
    for (unsigned int i = 0; i < matrixA->rows; i++) {
        for (unsigned int j = 0; j < matrixB->columns; j++) {
            for (unsigned int z = 0; z < matrixB->rows; z++) {
                multiplyMatrix->matrice[i][j] += matrixA->matrice[i][z] * matrixB->matrice[z][j];
            }
        }
    }
    
    return multiplyMatrix;
}


/*Column by column addition of a matrix with a column matrix*/
Matrix *columnAdditionMatrix(Matrix *matrix, Matrix *columnMatrix){
    if (matrix == NULL || columnMatrix == NULL){
        ErrorMemory("Column Addition Matrix")
    }
    
    if (matrix->rows != columnMatrix->rows || columnMatrix->columns != 1){
        ErrorSizeAddition(matrix->rows, matrix->columns, columnMatrix->rows, columnMatrix->columns, "Column Addition Matrix")
    }
    
    Matrix *sumMatrix = constructeurMatrix(matrix->rows, matrix->columns);
    
    for (unsigned int i = 0; i < matrix->rows; i++) {
        for (unsigned int j = 0; j < matrix->columns; j++) {
            sumMatrix->matrice[i][j] = matrix->matrice[i][j] + columnMatrix->matrice[i][0];
        }
    }
    
    return sumMatrix;
}


/*Row by Row addition of a matrix with a row matrix*/
Matrix *rowAdditionMatrix(Matrix *matrix, Matrix *rowMatrix){
    if (matrix == NULL || rowMatrix == NULL){
        ErrorMemory("Row addition Matrix")
    }
    
    if (matrix->columns != rowMatrix->columns || rowMatrix->rows != 1){
        ErrorSizeAddition(matrix->rows, matrix->columns, rowMatrix->rows, rowMatrix->columns, "Row Addition Matrix")
    }
    
    Matrix *sumMatrix = constructeurMatrix(matrix->rows, matrix->columns);
    
    for (unsigned int i = 0; i < matrix->rows; i++) {
        for (unsigned int j = 0; j < matrix->columns; j++) {
            sumMatrix->matrice[i][j] = matrix->matrice[i][j] + rowMatrix->matrice[0][j];
        }
    }
    
    return sumMatrix;
}


/*Column by column multiplication of a matrix by a column matrix*/
Matrix *columnMultiplyMatrix(Matrix *matrix, Matrix *columnMatrix){
    if (matrix == NULL || columnMatrix == NULL){
        ErrorMemory("Column Multiply Matrix")
    }
    
    if (matrix->rows != columnMatrix->rows || columnMatrix->columns != 1){
        ErrorSizeMultiplication(matrix->rows, matrix->columns, columnMatrix->rows, columnMatrix->columns, "Column Multiply Matrix")
    }
    
    Matrix *multiplyMatrix = constructeurMatrix(matrix->rows, matrix->columns);
    
    for (unsigned int i = 0; i < matrix->rows; i++) {
        for (unsigned int j = 0; j < matrix->columns; j++) {
            multiplyMatrix->matrice[i][j] = matrix->matrice[i][j] * columnMatrix->matrice[i][0];
        }
    }
    
    return multiplyMatrix;
}


/*Row by Row multiplication of a matrix by a row matrix*/
Matrix *rowMultiplyMatrix(Matrix *matrix, Matrix *rowMatrix){
    if (matrix == NULL || rowMatrix == NULL){
        ErrorMemory("Row Multiply Matrix")
    }
    
    if (matrix->columns != rowMatrix->columns || rowMatrix->rows != 1){
        ErrorSizeMultiplication(matrix->rows, matrix->columns, rowMatrix->rows, rowMatrix->columns, "Row Multiply Matrix")
    }
    
    Matrix *multiplyMatrix = constructeurMatrix(matrix->rows, matrix->columns);
    
    for (unsigned int i = 0; i < matrix->rows; i++) {
        for (unsigned int j = 0; j < matrix->columns; j++) {
            multiplyMatrix->matrice[i][j] = matrix->matrice[i][j] * rowMatrix->matrice[0][j];
        }
    }
    
    return multiplyMatrix;
}


/*Retrieve a column group*/
Matrix *getColumnsMatrix(Matrix *matrix, int startColumn, int endColumn){
    if (startColumn < 0 || endColumn < -1){
        ErrorParameter("Get Columns Matrix")
    }
    
    if (matrix == NULL){
        ErrorMemory("Get Columns Matrix")
    }
    
    endColumn = endColumn == -1 ? (int)matrix->columns - 1 : endColumn;
    endColumn = endColumn >= (int)matrix->columns ? (int)matrix->columns - 1 : endColumn;
    
    if (endColumn - startColumn < 0 || startColumn >= (int)matrix->columns){
        ErrorGetColumns(startColumn, endColumn, "Get Columns Matrix")
    }
    
    unsigned int nbColumns = endColumn - startColumn + 1;
    Matrix *getMatrixColumns = constructeurMatrix(matrix->rows, nbColumns);
    
    for (unsigned int i = 0; i < matrix->rows; i++) {
        for (unsigned int j = 0; j < nbColumns; j++) {
            getMatrixColumns->matrice[i][j] = matrix->matrice[i][j+startColumn];
        }
    }
    
    return getMatrixColumns;
}


/*Retrieve a row group*/
Matrix *getRowsMatrix(Matrix *matrix, int startRow, int endRow){
    if (startRow < 0 || endRow < -1){
        ErrorParameter("Get Rows Matrix")
    }
    
    if (matrix == NULL){
        ErrorMemory("Get Rows Matrix")
    }
    
    endRow = endRow == -1 ? (int)matrix->rows - 1 : endRow;
    endRow = endRow >= (int)matrix->rows ? (int)matrix->rows -1 : endRow;
    
    if (endRow - startRow < 0 || startRow > (int)matrix->rows){
        ErrorGetRows(startRow, endRow, "Get Rows Matrix")
    }
    
    unsigned int nbRows = endRow - startRow + 1;
    Matrix *getMatrixRows = constructeurMatrix(nbRows, matrix->columns);
    
    for (unsigned int i = 0; i < nbRows; i++) {
        for (unsigned int j = 0; j < matrix->columns; j++) {
            getMatrixRows->matrice[i][j] = matrix->matrice[i+startRow][j];
        }
    }
    
    return getMatrixRows;
}


/*Sum of all the columns of a matrix*/
Matrix *sumColumnsMatrix(Matrix *matrix){
    if(matrix == NULL){
        ErrorMemory("Sum Columns Matrix")
    }
    
    Matrix *sumMatrixColumns = constructeurMatrix(matrix->rows, 1);
    
    for (unsigned int i = 0; i < matrix->rows; i++) {
        for (unsigned int j = 0; j < matrix->columns; j++) {
            sumMatrixColumns->matrice[i][0] += matrix->matrice[i][j];
        }
    }
    
    return sumMatrixColumns;
}


/*Sum of all the rows of a matrix*/
Matrix *sumRowMatrix(Matrix *matrix){
    if(matrix == NULL){
        ErrorMemory("Sum Rows Matrix")
    }
    
    Matrix *sumMatrixRows = constructeurMatrix(1, matrix->columns);
    
    for (unsigned int i = 0; i < matrix->rows; i++) {
        for (unsigned int j = 0; j < matrix->columns; j++) {
            sumMatrixRows->matrice[0][j] += matrix->matrice[i][j];
        }
    }
    
    return sumMatrixRows;
}


/*Transpose of a matrix*/
Matrix *transpose(Matrix *matrix){
    if (matrix == NULL){
        ErrorMemory("Transpose")
    }
    
    Matrix *matrixTranspose = constructeurMatrix(matrix->columns, matrix->rows);
    
    for (unsigned int i = 0; i < matrix->rows; i++) {
        for (unsigned int j = 0; j < matrix->columns; j++) {
            matrixTranspose->matrice[j][i] = matrix->matrice[i][j];
        }
    }
    
    return matrixTranspose;
}


/*Create a random matrix*/
Matrix *randomMatrix(int rows, int columns, double interval){
    if (rows < 1 || columns < 1){
        ErrorParameter("Random Matrix")
    }
    
    Matrix *randomMatrix = constructeurMatrix(rows, columns);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            randomMatrix->matrice[i][j] = (((rand() % RAND_MAX) / (double)RAND_MAX) * 2 * interval) -interval;
        }
    }
    
    return randomMatrix;
}

/*Add matrix B in matrix A but skip the first column*/
void addMatrixSkipFirtsColumn(Matrix *matrixA, Matrix *matrixB){
    if (matrixA == NULL || matrixB == NULL){
        ErrorMemory("Add Matrix Skip First Column")
    }
    
    for (unsigned int i = 0; i < matrixA->rows; i++) {
        for (unsigned int j = 1; j < matrixA->columns; j++) {
            matrixA->matrice[i][j] += matrixB->matrice[i][j];
        }
    }
}


/*Create a copy of the matrix*/
Matrix *copyMatrix(Matrix *matrix){
    if (matrix == NULL){
        ErrorMemory("Copy Matrix")
    }
    
    Matrix *copyMatrix = constructeurMatrix(matrix->rows, matrix->columns);
    
    for (unsigned int i = 0; i < matrix->rows; i++) {
        for (unsigned int j = 0; j < matrix->columns; j++) {
            copyMatrix->matrice[i][j] = matrix->matrice[i][j];
        }
    }
    
    return copyMatrix;
}


/*Replace the column of a matrix by an other*/
Matrix *chanColumnMatrix(Matrix *matrix, int columnNumberToChange, Matrix *column){
    if (matrix == NULL || column == NULL){
        ErrorMemory("Change Column Matrix")
    }
    
    if (columnNumberToChange < 0 || matrix->rows != column->rows){
        ErrorParameter("Change Column Matrix")
    }
    
    for (unsigned int i = 0; i < matrix->rows; i++) {
        matrix->matrice[i][columnNumberToChange] = column->matrice[i][0];
    }
    
    destroyMatrix(column);
    return matrix;
}


/*Replace the row of a matrix by an other*/
Matrix *changeRowMatrix(Matrix *matrix, int rowNumberToChange, Matrix *row){
    if (matrix == NULL || row == NULL){
        ErrorMemory("Change Row Matrix")
    }
    
    if (rowNumberToChange < 0 || matrix->columns != row->columns){
        ErrorParameter("Change Row Matrix")
    }
    
    for (unsigned int i = 0; i < matrix->columns; i++) {
        matrix->matrice[rowNumberToChange][i] = row->matrice[0][i];
    }
    
    destroyMatrix(row);
    return matrix;
}


/*Edit a matrix by changing the values of a row (start a column 2) and adding a one (bias) at the begining of the matrix*/
void *editMatrixBychangingRowAdd1AtBegining(Matrix *matrix, int rowNumberToChange, Matrix *rowAdd){
    if (matrix == NULL || rowAdd == NULL){
        ErrorMemory("Edit Matrix By Changing Row Add 1 At Begining")
    }
    
    if (rowNumberToChange < 0 || rowAdd->columns != matrix->columns - 1){
        ErrorParameter("Edit Matrix By Changing Row Add 1 At Begining")
    }
    
    for (unsigned int i = 1; i < matrix->columns; i++) {
        matrix->matrice[rowNumberToChange][i] = rowAdd->matrice[0][i-1];
    }
    matrix->matrice[rowNumberToChange][0] = 1;
    
    destroyMatrix(rowAdd);
    return matrix;
}


/*Square matrix Elements and delete the first Column*/
Matrix *squareMatrixElementsWithoutFirstColumn(Matrix *matrix){
    if (matrix == NULL){
        ErrorMemory("Square Matrix Elements Without First Column")
    }
    
    Matrix *carreMatrix = constructeurMatrix(matrix->rows, matrix->columns - 1);
    
    for (unsigned int i = 0; i < matrix->rows; i++) {
        for (unsigned int j = 1; j < matrix->columns; j++) {
            carreMatrix->matrice[i][j-1] = matrix->matrice[i][j] * matrix->matrice[i][j];
        }
    }
    
    return carreMatrix;
}


/*Create a new matrix and add a column of 1 (bias) at the begining*/
Matrix *addColumnOnesBeginMatrix(Matrix *matrix){
    if (matrix == NULL){
        ErrorMemory("Add Column Ones Begin Matrix")
    }
    
    Matrix *newMatrix = constructeurMatrix(matrix->rows, matrix->columns + 1);
    
    for (unsigned int i = 0; i < matrix->rows; i++) {
        newMatrix->matrice[i][0] = 1;
        for (unsigned int j = 1; j < newMatrix->columns; j++) {
            newMatrix->matrice[i][j] = matrix->matrice[i][j-1];
        }
    }
    
    return newMatrix;
}


/*Replace the values of the matrix A by the values of the matrix B*/
void replaceValuesMatrix(Matrix *matrixA, Matrix *matrixB){
    if (matrixA->rows != matrixB->rows || matrixB->columns != matrixA->columns){
        ErrorSizeReplaceValues(matrixA->rows, matrixA->columns, matrixB->rows, matrixB->columns, "Replace Values Matrix")
    }
    
    for (unsigned int i = 0; i < matrixA->rows; i++) {
        for (unsigned int j = 0; j < matrixB->columns; j++) {
            matrixA->matrice[i][j] = matrixB->matrice[i][j];
        }
    }
    
    destroyMatrix(matrixB);
}

Matrix *transformVectorColumnMatrix(Matrix *matrix){
	Matrix *new = constructeurMatrix(1, matrix->rows * matrix->columns + 1);
	new->matrice[0][0] = 1;

	for(unsigned int i = 0; i < matrix->rows; i++){
		for(unsigned int j = 0; j < matrix->columns; j++){
			new->matrice[0][i * matrix->columns + j + 1] = matrix->matrice[i][j];
		}
	}

	return new;
}
