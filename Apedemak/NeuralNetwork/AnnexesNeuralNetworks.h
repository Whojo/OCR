//
//  AnnexesNeuralNetworks.h
//  OCR
//
//  Created by Moustapha Diop on 22/11/2019.
//  Copyright © 2019 Moustapha Diop. All rights reserved.
//

#ifndef AnnexesNeuralNetworks_h
#define AnnexesNeuralNetworks_h

#include <stdio.h>

#include "Matrix.h"
#include "Functions.h"
#include "../Segmentation/segmentation_G.h"

/*Parser of Data*/
void parser(char wayToOpenFile[200], Matrix *XElement, Matrix *YElement);

/*Return the precsion the neural network with the inputs*/
double PresicionNeuralNetwork(Matrix *thetaOne, Matrix *thetaTwo, Matrix *XElements, Matrix *YElements);

/*Save Theta1 and Theta2 after minJ(theta)*/
void SaveThetasOfNeuralNetwork(char wayToSaveThetas[200], Matrix *theta1, Matrix *theta2, int pourcentage, double lambda);

/*Retrieve Theta1 and Theta2 for the neural network*/
void RetrieveThetasForNeuralNetwork(char wayToSaveThetas[200], Matrix *theta1, Matrix *theta2);

/*Load a matrix image in a file*/
Matrix *loadImageMatrixForNeuralNetwork(char wayToFindImage[200]);

/*Find the correspondant Element all NN*/
char ApedemakNeuralNetwork(Matrix *data, char wayOpenDigit[200], char wayOpenLettMin[200], char wayOpenLettMaj[200], char wayToSave[200]);

/*Find the prediction all NN*/
char _ApedemakNeuralNetwork(Matrix *data, Matrix *Theta1Digit, Matrix *Theta2Digit, Matrix *Theta1LettMaj, Matrix *Theta2LettMaj, Matrix *Theta1LettMin, Matrix *Theta2LettMin);

/*Make the prediction and put them in a file*/
int ApedemakMain(struct par *prediction, char wayToSave[200], char wayToOpen[200], char type);

/*Do the prediction*/
char _Apedemak(Matrix *prediction, Matrix *ThetaOne, Matrix *ThetaTwo, char type);




#endif /* AnnexesNeuralNetworks_h */

