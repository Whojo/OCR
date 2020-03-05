//
//  Functions.h
//  OCR
//
//  Created by Moustapha Diop on 16/09/2019.
//  Copyright © 2019 Moustapha Diop. All rights reserved.
//

#ifndef Functions_h
#define Functions_h

#include <stdio.h>
#include <math.h>
#include "Matrix.h"


/*Sigmoid function*/
// Check and double check
double sigmoid(double inputDouble);

/*Compute the sigmoid function in a matrix*/
// Check and double check
Matrix *sigmoidMatrix(Matrix *matrix);

/*Compute the log fucntion in a matrix*/
// Check and double check
Matrix *logFunctionMatrix(Matrix *matrix);

/*Compute Feedforward propagation to get h(x) -> change a2, a3 = h(x)*/
// Check and double check
void feedForward(Matrix *inputLayer, Matrix *hiddenLayer, Matrix *trainingElementsX, Matrix *a2, Matrix *a3);

/*Compute the cost function = J(Theta)*/
// Check and double check
double costFunction(Matrix *inputLayer, Matrix *hiddenLayer, Matrix *trainingElementsX, Matrix *resultsY, double lambda, Matrix *a3);

/*Compute Backpropagation Algorithm*/
// Check and double check
void backpropagation(Matrix *a2, Matrix *a3, Matrix *inputLayer, Matrix *hiddenLayer, Matrix *resultsY, Matrix *trainingElementsX, double lambda);

/*Compute Gradient Descent with Backpropagation*/
// Check and double check
void gradientDescent(Matrix *theta1, Matrix *theta2, unsigned int maxIter, Matrix *trainingElementsX, Matrix *resultsY, double lambda, double precision);

/*Prediction*/
// Check and double check
int Apedemak(Matrix *prediction, Matrix *theta1, Matrix *theta2);


#endif /* Functions_h */
