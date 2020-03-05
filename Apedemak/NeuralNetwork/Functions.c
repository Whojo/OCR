//
//  Functions.c
//  OCR
//
//  Created by Moustapha Diop on 16/09/2019.
//  Copyright © 2019 Moustapha Diop. All rights reserved.
//

#include "Functions.h"

#define ErrorMemory(memory) \
printf("Error Memory : Cannot access to this memory : This error comes from -> %s \n", memory); \
exit(0);

#define ErrorResults(function) \
printf("Error Results : The results is bigger than the number of units in the last layer : This error comes from -> %s \n", function); \
exit(0);

#define ErrorLambda(function) \
printf("Error Lambda : Lambda cannot be negative : This error comes from -> %s \n", function); \
exit(0);



/*Sigmoid function*/
double sigmoid(double inputDouble){
    return (1.0/(1.0 + exp(-inputDouble)));
}


/*Compute the sigmoid function in a matrix*/
Matrix *sigmoidMatrix(Matrix *matrix){
    if(matrix == NULL){
        ErrorMemory("Sigmoid Matrix")
    }
    
    for (unsigned int i = 0; i < matrix->rows; i++) {
        for (unsigned int j = 0; j < matrix->columns; j++) {
            matrix->matrice[i][j] = sigmoid(matrix->matrice[i][j]);
        }
    }
    
    return matrix;
}


/*Compute the log fucntion in a matrix*/
Matrix *logFunctionMatrix(Matrix *matrix){
    if(matrix == NULL){
        ErrorMemory("Log Function Matrix")
    }
    
    for (unsigned int i = 0; i < matrix->rows; i++) {
        for (unsigned int j = 0; j < matrix->columns; j++) {
            matrix->matrice[i][j] = log(matrix->matrice[i][j]);
        }
    }
    
    return matrix;
}


/*Compute Feedforward propagation to get h(x) -> change a2, a3 = h(x)*/
void feedForward(Matrix *inputLayer, Matrix *hiddenLayer, Matrix *trainingElementsX, Matrix *a2, Matrix *a3){
    if (inputLayer == NULL || hiddenLayer == NULL || trainingElementsX == NULL || a2 == NULL || a3 == NULL){
        ErrorMemory("Feed Forward")
    }
    
    /*
     * Initialisation of a2_ and a3_
     * They will become a2 and a3
     */
    Matrix *a2_ = constructeurMatrix(trainingElementsX->rows, inputLayer->rows + 1);
    Matrix *a3_ = constructeurMatrix(trainingElementsX->rows, hiddenLayer->rows);

    // Loop that calculates a2 = g(theta1 * X)
    for (unsigned int i = 0; i < trainingElementsX->rows; i++) {
        
        /*Steps to calculate a2
         * 1- Get the right row of trainingSets
         * 2- Multiply every row of inputLayers by the row of trainingSets
         * 3- Add column by column inputsLayers to get a row column
         * 4- Do the transpose of this column matrix in order to have a row matrix and add it in a2_
         */
        
        Matrix *getRowTrainingSet = getRowsMatrix(trainingElementsX, i, i); // Step 1-
        Matrix *trainingSetsXInputLayer = rowMultiplyMatrix(inputLayer, getRowTrainingSet); // Step 2-

        Matrix *pivoSum = sumColumnsMatrix(trainingSetsXInputLayer); // Step 3-
        Matrix *a2Row = transpose(sigmoidMatrix(pivoSum)); // Step 4-

        /*Add a one at the begining (bias)*/
        editMatrixBychangingRowAdd1AtBegining(a2_, i, a2Row); // Step 4-
        
        //Destroy matrices
        destroyMatrix(getRowTrainingSet);
        destroyMatrix(trainingSetsXInputLayer);
        destroyMatrix(pivoSum);
    }
    
    
    // Loop that calculates a3 = h(x) = g(theta2 * a2)
    for (unsigned int i = 0; i < trainingElementsX->rows; i++) {
        
        /*Steps to calculate a3 = h(x)
         * 1- Get the right row of a2_
         * 2- Multiply every row of hiddenLayers by the row of a2_
         * 3- Add column by column hiddenLayers to get a row column
         * 4- Do the transpose of this column matrix in order to have a row matrix and add it in a3_
         */
        
        Matrix *getRowA2 = getRowsMatrix(a2_, i, i); // Step 1-
        Matrix *a2XHiddenLayer = rowMultiplyMatrix(hiddenLayer, getRowA2); // Step 2-
        
        Matrix *pivoA2HiddenLayer = sumColumnsMatrix(a2XHiddenLayer); // Step 3-
        Matrix *a3Row = transpose(sigmoidMatrix(pivoA2HiddenLayer)); // Step 4-
        
        changeRowMatrix(a3_, i, a3Row); // Step 4-
        
        //Destroy matrices
        destroyMatrix(getRowA2);
        destroyMatrix(a2XHiddenLayer);
        destroyMatrix(pivoA2HiddenLayer);
    }
    
    /* Update the values a2 and a3 = h(x) given in parameter*/
    
    replaceValuesMatrix(a2, a2_);
    replaceValuesMatrix(a3, a3_);
}


/*Compute the Cost Function = J(theta)*/
double costFunction(Matrix *inputLayer, Matrix *hiddenLayer, Matrix *trainingElementsX, Matrix *arrayResultsY, double lambda, Matrix *a3){
    if (inputLayer == NULL || hiddenLayer == NULL || trainingElementsX == NULL || arrayResultsY == NULL || a3 == NULL){
        ErrorMemory("Cost Function")
    }
    
    if (lambda < 0.0){
        ErrorLambda("costFunction")
    }
    
    Matrix *Theta1 = squareMatrixElementsWithoutFirstColumn(inputLayer);
    Matrix *Theta2 = squareMatrixElementsWithoutFirstColumn(hiddenLayer);
    
    /*Cost function J(theta)*/
    double J;
    
    // Create two copies of a3 in order to calculate J without modify a3;
    Matrix *copyA3_1 = copyMatrix(a3);
    Matrix *copyA3_2 = copyMatrix(a3);
    
    //Create copies of arrayResultsY
    Matrix *copyArrayResultsY_1 = copyMatrix(arrayResultsY);
    Matrix *copyArrayResulstY_2 = copyMatrix(arrayResultsY);
    
    //Begining of the calcul of J
    logFunctionMatrix(scalarAdditionMatrix(scalarMultiplyMatrix(copyA3_1, -1), 1)); //copyA3_1 = log(1 .- a3)
    scalarAdditionMatrix(scalarMultiplyMatrix(copyArrayResultsY_1, -1), 1); //copyTableauResultsY_1 = (1 .- newY)
    logFunctionMatrix(copyA3_2); //copyA3_2 = (log(a3))
    scalarMultiplyMatrix(copyArrayResulstY_2, -1); //copyTableauResulstY_2 = -newy
    
    Matrix *copyA3_2xcopyArrayResultsY2 = MultiplyElementsMatrix(copyA3_2, copyArrayResulstY_2); //-newy .* log(a3)
    Matrix *copyA3_1xcopyArrayResultsY1 = MultiplyElementsMatrix(copyArrayResultsY_1, copyA3_1); //(-1 .- newy) .* log(1 .- a3)
    
    Matrix *substractCopysArray = subtractMatrix(copyA3_2xcopyArrayResultsY2, copyA3_1xcopyArrayResultsY1);
    Matrix *sumRow = sumRowMatrix(substractCopysArray);
    Matrix *doubleMatrix = sumColumnsMatrix(sumRow);
    
    // First part of J without the term of regularization
    J = (1.0/trainingElementsX->rows) * doubleMatrix->matrice[0][0];
    
    
    //Destroy Matrix
    destroyMatrix(copyA3_1);
    destroyMatrix(copyA3_2);
    destroyMatrix(copyArrayResultsY_1);
    destroyMatrix(copyArrayResulstY_2);
    destroyMatrix(copyA3_1xcopyArrayResultsY1);
    destroyMatrix(copyA3_2xcopyArrayResultsY2);
    destroyMatrix(substractCopysArray);
    destroyMatrix(sumRow);
    destroyMatrix(doubleMatrix);
    
    
    //Adding the term of regularization of J
    Matrix *sumRowTheta1 = sumRowMatrix(Theta1);
    Matrix *sumRowTheta2 = sumRowMatrix(Theta2);
    Matrix *doubleTheta1 = sumColumnsMatrix(sumRowTheta1);
    Matrix *doubleTheta2 = sumColumnsMatrix(sumRowTheta2);
    
    //Adding regularization -> end calcul J
    J += (lambda / (2.0 * trainingElementsX->rows)) * (doubleTheta1->matrice[0][0] + doubleTheta2->matrice[0][0]);
    
    destroyMatrix(sumRowTheta1);
    destroyMatrix(sumRowTheta2);
    destroyMatrix(doubleTheta1);
    destroyMatrix(doubleTheta2);
    destroyMatrix(Theta1);
    destroyMatrix(Theta2);
   
    return J;
}


/*Compute Backpropagation Algorithm*/
void backpropagation(Matrix *a2, Matrix *a3, Matrix *inputLayer, Matrix *hiddenLayer, Matrix *arrayResultsY, Matrix *trainingElementsX, double lambda){
    if (a2 == NULL || a3 == NULL || inputLayer == NULL || hiddenLayer == NULL || arrayResultsY == NULL || trainingElementsX == NULL){
        ErrorMemory("Back Propagation")
    }
    
    if (lambda < 0.0){
        ErrorLambda("Back Propagation")
    }
    
    /*For the output layer*/
    Matrix *yL3 = subtractMatrix(a3, arrayResultsY);
    
    /*For the hidden layer*/
    Matrix *yL2 = constructeurMatrix(trainingElementsX->rows, hiddenLayer->columns);
    
    /*Do the transpose of Theta2 in order to have the good size*/
    Matrix *transposeTheta2 = transpose(hiddenLayer);
    
    
    //Loop that calculates yL2 = (theta2)T*yL3
    for (unsigned int i = 0; i < trainingElementsX->rows; i++) {
        
        /*Steps to calculate yL2 = (theta2)T*yL3
         * 1- Get the right row of yL3
         * 2- Multiply Theta2 by the row of a3
         * 3- Sum of the columns and transpose the matrix to integrate it in yL2
         */
        
        Matrix *getYL3Row = getRowsMatrix(yL3, i, i); // Step 1-
        Matrix *multiplyTheta2TranXgetYL3 = rowMultiplyMatrix(transposeTheta2, getYL3Row); // Step 2-
        
        Matrix *sumMultiply = sumColumnsMatrix(multiplyTheta2TranXgetYL3); // Step 3-
        Matrix *transposeSum = transpose(sumMultiply); // Step 3-
        
        //Add the result in yL2
        changeRowMatrix(yL2, i, transposeSum); // Step 3-
        
        //Destroy Matrix
        destroyMatrix(getYL3Row);
        destroyMatrix(multiplyTheta2TranXgetYL3);
        destroyMatrix(sumMultiply);
    }
    destroyMatrix(transposeTheta2);
    
    //Copy of a2
    Matrix *a2Copy = copyMatrix(a2);

    
    //Modification of yL2 -> yL2 = yL2 .* g'(z2) = yL2 .* a2 .* (1 - a2)
    Matrix *yL2Xa2 = MultiplyElementsMatrix(yL2, a2);
    scalarAdditionMatrix(scalarMultiplyMatrix(a2Copy, -1), 1); // 1 - a2
    Matrix *newYL2 = MultiplyElementsMatrix(yL2Xa2, a2Copy);
    
    
    //Destroy Matrixs
    destroyMatrix(yL2Xa2);
    destroyMatrix(yL2);
    destroyMatrix(a2Copy);
    
    
    //Acumulate the gradient
    Matrix *transposeYL2 = transpose(newYL2);
    Matrix *delta2Bis = MultiplyMatrix(transposeYL2, trainingElementsX);
    destroyMatrix(transposeYL2);
    
    
    //Supression of the first row of multiplyTransposeTrainingElementX
    Matrix *delta2_ = getRowsMatrix(delta2Bis, 1, -1);
    destroyMatrix(delta2Bis);
    
    
    Matrix *transposeYL3 = transpose(yL3);
    Matrix *delta3_ = MultiplyMatrix(transposeYL3, a2);
    
    destroyMatrix(transposeYL3);

    //Regularized Neural Network -> regularized gradient
    double lam = lambda/trainingElementsX->rows;
    double oneAbovem = 1.0/trainingElementsX->rows;
    
    Matrix *theta1Grad = scalarMultiplyMatrix(delta2_, oneAbovem);
    Matrix *theta2Grad = scalarMultiplyMatrix(delta3_, oneAbovem);
    
    Matrix *theta2Copy = copyMatrix(hiddenLayer);
    Matrix *theta1Copy = copyMatrix(inputLayer);
    
    //Add regularized terms
    addMatrixSkipFirtsColumn(theta2Grad, scalarMultiplyMatrix(theta2Copy, lam));
    addMatrixSkipFirtsColumn(theta1Grad, scalarMultiplyMatrix(theta1Copy, lam));

    //Update theta1 = inputLayer and theta2 = hiddenLayer
    replaceValuesMatrix(inputLayer, subtractMatrix(inputLayer, theta1Grad));
    replaceValuesMatrix(hiddenLayer, subtractMatrix(hiddenLayer, theta2Grad));
    
    
    //Destroy matrices
    destroyMatrix(yL3);
    destroyMatrix(newYL2);
    destroyMatrix(delta2_);
    destroyMatrix(delta3_);
    destroyMatrix(theta1Copy);
    destroyMatrix(theta2Copy);
}


/*Compute Gradient Descent with Backpropagation*/
void gradientDescent(Matrix *theta1, Matrix *theta2, unsigned int maxIter, Matrix *trainingElementsX, Matrix *resultsY, double lambda, double precision){
    for (unsigned int i = 0; i < theta1->rows; i++) {
        theta1->matrice[i][0] = 1;
    }
    
    Matrix *a2 = constructeurMatrix(trainingElementsX->rows, theta1->rows + 1);
    Matrix *a3 = constructeurMatrix(trainingElementsX->rows, theta2->rows);
    
    /*Create a new array of the resultats Y*/
    Matrix *arrayResultsY = constructeurMatrix(trainingElementsX->rows, theta2->rows);
    for (unsigned int i = 0; i < trainingElementsX->rows; i++) {
        int y = resultsY->matrice[i][0];
        if (y > (int)arrayResultsY->columns){
            ErrorResults("Gradient Descent")
        }
        arrayResultsY->matrice[i][y] = 1;
    }
    
    double J = 0;
    double K = 0;
    unsigned int i = 0;
    double interval = K - J;
    
    feedForward(theta1, theta2, trainingElementsX, a2, a3);
    J = costFunction(theta1, theta2, trainingElementsX, arrayResultsY, lambda, a3);
    backpropagation(a2, a3, theta1, theta2, arrayResultsY, trainingElementsX, lambda);

    
    do{
        feedForward(theta1, theta2, trainingElementsX, a2, a3);
        K = costFunction(theta1, theta2, trainingElementsX, arrayResultsY, lambda, a3);
        backpropagation(a2, a3, theta1, theta2, arrayResultsY, trainingElementsX, lambda);
        
        interval = K - J;
        J = K;
        
        printf("Nb iteration = %d, J = %lf\n",i, J);

        i++;
    }while (i < maxIter && (interval < -precision));
    
    printf("Cost function = %f, lambda = %f \n", J, lambda);
    
    destroyMatrix(arrayResultsY);
    destroyMatrix(a2);
    destroyMatrix(a3);
}


/*Prediction*/
int Apedemak(Matrix *prediction, Matrix *theta1, Matrix *theta2){
    if (prediction == NULL || theta1 == NULL || theta2 == NULL){
        ErrorMemory("Apedemak")
    }
    
    Matrix *newTheta1 = transpose(theta1);
    Matrix *newTheta2 = transpose(theta2);

    Matrix *layer = sigmoidMatrix(MultiplyMatrix(prediction, newTheta1));
    Matrix *hiddenLayer = addColumnOnesBeginMatrix(layer);
    destroyMatrix(newTheta1);
    destroyMatrix(layer);
    
    Matrix *outputLayer = sigmoidMatrix(MultiplyMatrix(hiddenLayer, newTheta2));
    destroyMatrix(hiddenLayer);
    destroyMatrix(newTheta2);
    
    int predic = 0;
    for (unsigned int i = 0; i < outputLayer->columns; i++) {
        if (outputLayer->matrice[0][predic] < outputLayer->matrice[0][i]){
            predic = i;
        }
    }
    
    destroyMatrix(outputLayer);
    return predic;
}
