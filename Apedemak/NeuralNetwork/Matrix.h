//
//  Matrix.h
//  OCR
//
//  Created by Moustapha Diop on 14/09/2019.
//  Copyright © 2019 Moustapha Diop. All rights reserved.
//

#ifndef Matrix_h
#define Matrix_h

#include <stdio.h>
#include <stdlib.h>

// Struct of matrices
// Check and double check
typedef struct Matrix Matrix;
struct Matrix {
    unsigned int rows;
    unsigned int columns;
    double **matrice;
};

/*Return a matrix rows by columns with 0*/
// Check and double check
Matrix *constructeurMatrix(int rows, int columns);

/*Print the matrix in parameter*/
// Check and double check
void printMatrix(Matrix *matrix);

/*Create a personalize matrix*/
// Check and double check
Matrix *createMatrix(int row, int columns, double listeMatrix[]);

/*Free memory by destroying the matrix*/
// Check and double check
void destroyMatrix(Matrix *matrix);

/*Multiplication of the matrix by a double*/
// Check and double check
Matrix *scalarMultiplyMatrix(Matrix *matrix, double scalar);

/*Addition of the matrix by a double*/
// Check and double check
Matrix *scalarAdditionMatrix(Matrix *matrix, double scalar);

/*Addition of two matrices*/
// Check and double check
Matrix *AdditionMatrix(Matrix *matrixA, Matrix *matrixB);

/*Substraction of two matrices*/
// Check and double check
Matrix *subtractMatrix(Matrix *matrixA, Matrix *matrixB);

/*Multiplication value by value of the matrices*/
// Check and double check
Matrix *MultiplyElementsMatrix(Matrix *matrixA, Matrix *matrixB);

/*Multiplication of two matrices*/
// Check and double check
Matrix *MultiplyMatrix(Matrix *matrixA, Matrix *matrixB);

/*Column by column addition of a matrix with a column matrix*/
// Check and double check
Matrix *columnAdditionMatrix(Matrix *matrix, Matrix *columnMatrix);

/*Row by Row addition of a matrix with a row matrix*/
// Check and double check
Matrix *rowAdditionMatrix(Matrix *matrix, Matrix *rowMatrix);

/*Column by column multiplication of a matrix by a column matrix*/
// Check and double check
Matrix *columnMultiplyMatrix(Matrix *matrix, Matrix *columnMatrix);

/*Row by Row multiplication of a matrix by a row matrix*/
// Check and double check
Matrix *rowMultiplyMatrix(Matrix *matrix, Matrix *rowMatrix);

/*Retrieve a column group*/
// Check and double check
Matrix *getColumnsMatrix(Matrix *matrix, int startColumn, int endColumn);

/*Retrieve a row group*/
// Check and double check
Matrix *getRowsMatrix(Matrix *matrix, int startRow, int endRow);

/*Sum of all the columns of a matrix*/
// Check and double check
Matrix *sumColumnsMatrix(Matrix *matrix);

/*Sum of all the rows of a matrix*/
// Check and double check
Matrix *sumRowMatrix(Matrix *matrix);

/*Transpose of a matrix*/
// Check and double check
Matrix *transpose(Matrix *matrix);

/*Create a random matrix between the interval [-interval; interval]*/
// Check and double check
Matrix *randomMatrix(int rows, int columns, double interval);

/*Add matrix B in matrix A but skip the first column*/
// Check and double check
void addMatrixSkipFirtsColumn(Matrix *matrixA, Matrix *matrixB);

/*Create a copy of the matrix*/
// Check and double check
Matrix *copyMatrix(Matrix *matrix);

/*Replace the column of a matrix by an other*/
// Check and double check
Matrix *chanColumnMatrix(Matrix *matrix, int columnNumberToChange, Matrix *column);

/*Replace the row of a matrix by an other*/
// Check and double check
Matrix *changeRowMatrix(Matrix *matrix, int rowNumberToChange, Matrix *row);

/*Edit a matrix by changing the values of a row and adding a one (bias) at the begining of the matrix*/
// Check and double check
void *editMatrixBychangingRowAdd1AtBegining(Matrix *matrix, int rowNumberToChange, Matrix *rowAdd);

/*Square matrix Elements and delete the first Column*/
// Check and double check
Matrix *squareMatrixElementsWithoutFirstColumn(Matrix *matrix);

/*Create a new matrix and add a column of 1 (bias) at the begining*/
// Check and double check
Matrix *addColumnOnesBeginMatrix(Matrix *matrix);

/*Replace the values of the matrix A by the values of the matrix B*/
// Check and double check
void replaceValuesMatrix(Matrix *matrixA, Matrix *matrixB);

Matrix *transformVectorColumnMatrix(Matrix *matrix);


#endif /* Matrix_h */
