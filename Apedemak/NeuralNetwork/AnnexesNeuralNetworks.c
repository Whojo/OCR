//
//  AnnexesNeuralNetworks.c
//  OCR
//
//  Created by Moustapha Diop on 22/11/2019.
//  Copyright © 2019 Moustapha Diop. All rights reserved.
//

#include "AnnexesNeuralNetworks.h"

#define ErrorNotExist(name_file) \
printf("ErrorNotExist : This file '%s' does not exist. \n", name_file); \
exit(0);

#define ErrorMemory(memory) \
printf("Error Memory : Cannot access to this memory : This error comes from -> %s \n", memory); \
exit(0);

#define ErrorFscanf(where, what) \
printf("Error Fscanf : This error comes from -> %s : %s\n", where, what); \
exit(0);


void parser(char wayToOpenFile[200], Matrix *XElement, Matrix *YElement){
    FILE *fileContainDigits = NULL;
    fileContainDigits = fopen(wayToOpenFile, "r");
    
    if (fileContainDigits == NULL){
        ErrorNotExist(wayToOpenFile)
    }
    //Get the number of trainin examples
    unsigned int numberOfExamples = (int) XElement->rows;
    unsigned int MAX = (int)XElement->columns;
    
    int valeur;
    double pixel;
    
    for(unsigned int i = 0; i < numberOfExamples; i++){
        /*if (!fscanf(fileContainDigits, "%d", &valeur))
            ErrorFscanf("Parser", "YValue")*/
        fscanf(fileContainDigits, "%d", &valeur);
        YElement->matrice[i][0] = valeur;
        
        for (unsigned int j = 0; j < MAX; j++) {
            /*printf("bon passage\n");
            if (!fscanf(fileContainDigits, "%lf", &pixel)){
                printf("bon passage\n");
                ErrorFscanf("Parser", "XPixel")
            }*/
            fscanf(fileContainDigits, "%lf", &pixel);
            XElement->matrice[i][j] = pixel;
        }
    }
    
    fclose(fileContainDigits);
}


/*Return the precsion the neural network with the inputs*/
double PresicionNeuralNetwork(Matrix *thetaOne, Matrix *thetaTwo, Matrix *XElements, Matrix *YElements){
    if (thetaOne == NULL || thetaTwo == NULL || XElements == NULL || YElements == NULL){
        ErrorMemory("Precision Of Neural Network")
    }
    
    int numberGoodJob = 0;
    for(unsigned int i = 0; i < XElements->rows; i++){
        Matrix *getRightRow = getRowsMatrix(XElements, i, i);
        if (Apedemak(getRightRow, thetaOne, thetaTwo) == (int) YElements->matrice[i][0]){
            numberGoodJob++;
        }
        
        destroyMatrix(getRightRow);
    }
    
    return ((((double) numberGoodJob) / XElements->rows) * 100);
}


/*Save Theta1 and Theta2 after minJ(theta)*/
void SaveThetasOfNeuralNetwork(char wayToSaveThetas[200], Matrix *theta1, Matrix *theta2, int pourcentage, double lambda){
    FILE* file = NULL;
    
    file = fopen(wayToSaveThetas, "w");
    if (file == NULL){
        ErrorNotExist(wayToSaveThetas)
    }
    
    fprintf(file, "%d\n", pourcentage);
    fprintf(file, "%f\n", lambda);
    
    fprintf(file, "%d %d\n", theta1->rows, theta1->columns);
    
    for (unsigned int i = 0; i < theta1->rows; i++) {
        for (unsigned int j = 0; j < theta1->columns - 1; j++) {
            fprintf(file, "%f ", theta1->matrice[i][j]);
        }
        fprintf(file, "%f\n", theta1->matrice[i][theta1->columns-1]);
    }
    
    fprintf(file, "%d %d\n", theta2->rows, theta2->columns);
    
    for (unsigned int i = 0; i < theta2->rows; i++) {
        for (unsigned int j = 0; j < theta2->columns - 1; j++) {
            fprintf(file, "%f ", theta2->matrice[i][j]);
        }
        fprintf(file, "%f\n", theta2->matrice[i][theta2->columns-1]);
    }
    fclose(file);
}



/*Retrieve Theta1 and Theta2 for the neural network*/
void RetrieveThetasForNeuralNetwork(char wayToSaveThetas[200], Matrix *theta1, Matrix *theta2){
    FILE *file = NULL;
    
    file = fopen(wayToSaveThetas, "r");
    if (file == NULL){
        ErrorNotExist(wayToSaveThetas)
    }
    
    int pourcentage;
    double lambda;
    unsigned int thetaRow;
    unsigned int thetaColumn;
    
    double valeur;
    
    if (!fscanf(file, "%d", &pourcentage)){
		ErrorFscanf("Retrieve Thetas", "Pourcentage")
	}
    if (!fscanf(file, "%lf", &lambda)){
		ErrorFscanf("Retrieve Thetas", "Lambda")
	}
    if (!fscanf(file, "%u", &thetaRow)){
		ErrorFscanf("Retrieve Thetas", "Theta1Row")
	}
    if(!fscanf(file, "%u", &thetaColumn)){
		ErrorFscanf("Retrieve Thetas", "Theta1Column")
	}
    
    for (unsigned int i = 0; i < thetaRow; i++) {
        for (unsigned int j = 0; j < thetaColumn; j++) {
            if (!fscanf(file, "%lf", &valeur)){
				ErrorFscanf("Retrieve Thetas", "Values1")
			}
            theta1->matrice[i][j] = valeur;
        }
    }
    
    if (!fscanf(file, "%u", &thetaRow)){
		ErrorFscanf("Retrieve Thetas", "Theta2Row")
	}
    if (!fscanf(file, "%u", &thetaColumn)){
		ErrorFscanf("Retrieve Thetas", "Theta2Column")
	}
    
    for (unsigned int i = 0; i < thetaRow; i++) {
        for (unsigned int j = 0; j < thetaColumn; j++) {
            if (!fscanf(file, "%lf", &valeur)){
				ErrorFscanf("Retrieve Thetas", "Values2")
			}
            theta2->matrice[i][j] = valeur;
        }
    }
    
    fclose(file);
}


/*Load a matrix image in a file*/
Matrix *loadImageMatrixForNeuralNetwork(char wayToFindImage[200]){
    FILE *file = NULL;
    
    file = fopen(wayToFindImage, "r");
    if (file == NULL){
        ErrorNotExist(wayToFindImage)
    }
    
    unsigned int ImageRow;
    unsigned int ImageColumn;
    
    double valeur;
    
    fscanf(file, "%d", &ImageRow);
    fscanf(file, "%d", &ImageColumn);
    
    Matrix *Image = constructeurMatrix(1, ImageColumn * ImageColumn);
    
    for (unsigned int i = 0; i < ImageRow * ImageColumn; i++) {
        fscanf(file, "%lf", &valeur);
        Image->matrice[0][i] = valeur;
    }
    
    return Image;
}


char _ApedemakNeuralNetwork(Matrix *data, Matrix *Theta1Digit, Matrix *Theta2Digit, Matrix *Theta1LettMaj, Matrix *Theta2LettMaj, Matrix *Theta1LettMin, Matrix *Theta2LettMin){
    if (data == NULL || Theta1Digit == NULL || Theta2Digit == NULL || Theta1LettMaj == NULL || Theta2LettMaj == NULL || Theta1LettMin == NULL || Theta2LettMin == NULL){
        ErrorMemory("ApedemakNeuralNetwork")
    }
    
    Matrix *layer = sigmoidMatrix(MultiplyMatrix(data, Theta1Digit));
    Matrix *hiddenLayer = addColumnOnesBeginMatrix(layer);
    destroyMatrix(layer);
    
    Matrix *outputLayer = sigmoidMatrix(MultiplyMatrix(hiddenLayer, Theta2Digit));
    destroyMatrix(hiddenLayer);
    
    int predicDigit = 0;
    double valeurDigit = 0.0;
    for (unsigned int i = 0; i < outputLayer->columns; i++) {
        if (outputLayer->matrice[0][predicDigit] < outputLayer->matrice[0][i]){
            predicDigit = i;
            valeurDigit = outputLayer->matrice[0][i];
        }
    }
    
    destroyMatrix(outputLayer);

    
    Matrix *layer1 = sigmoidMatrix(MultiplyMatrix(data, Theta1LettMaj));
    Matrix *hiddenLayer1 = addColumnOnesBeginMatrix(layer1);
    destroyMatrix(layer1);
    
    Matrix *outputLayer1 = sigmoidMatrix(MultiplyMatrix(hiddenLayer1, Theta2LettMaj));
    destroyMatrix(hiddenLayer1);
    
    int predicLettMaj = 0;
    double valeurLettMaj = 0.0;
    for (unsigned int i = 0; i < outputLayer1->columns; i++) {
        if (outputLayer1->matrice[0][predicLettMaj] < outputLayer1->matrice[0][i]){
            predicLettMaj = i;
            valeurLettMaj = outputLayer1->matrice[0][i];
        }
    }
    
    destroyMatrix(outputLayer1);
    
    
    Matrix *layer2 = sigmoidMatrix(MultiplyMatrix(data, Theta1LettMin));
    Matrix *hiddenLayer2 = addColumnOnesBeginMatrix(layer2);
    destroyMatrix(layer2);
    
    Matrix *outputLayer2 = sigmoidMatrix(MultiplyMatrix(hiddenLayer2, Theta2LettMin));
    destroyMatrix(hiddenLayer2);
    
    int predicLettMin = 0;
    double valeurLettMin = 0.0;
    for (unsigned int i = 0; i < outputLayer2->columns; i++) {
        if (outputLayer2->matrice[0][predicLettMin] < outputLayer2->matrice[0][i]){
            predicLettMin = i;
            valeurLettMin = outputLayer2->matrice[0][i];
        }
    }
    
    destroyMatrix(outputLayer2);
    
    
    if (valeurDigit > valeurLettMaj || valeurDigit > valeurLettMin){
        return (predicDigit + 48);
    }
    
    if (valeurLettMin > valeurLettMaj || valeurLettMin > valeurDigit){
        return (predicLettMin + 97);
    }
    
    return (predicLettMaj + 65);
}

/*Find the correspondant Element*/
char ApedemakNeuralNetwork(Matrix *data, char wayOpenDigit[200], char wayOpenLettMin[200], char wayOpenLettMaj[200], char wayToSave[200]){
    
    Matrix *theta1DigitTrans = constructeurMatrix(30, 785);
    Matrix *theta2DigitTrans = constructeurMatrix(10, 31);
    RetrieveThetasForNeuralNetwork(wayOpenDigit, theta1DigitTrans, theta2DigitTrans);
    Matrix *ThetaOneDigit = transpose(theta1DigitTrans);
    Matrix *ThetaTwoDigit = transpose(theta2DigitTrans);
    destroyMatrix(theta2DigitTrans);
    destroyMatrix(theta1DigitTrans);
    
    Matrix *theta1LettMin = constructeurMatrix(40, 785);
    Matrix *theta2LettMin = constructeurMatrix(26, 41);
    RetrieveThetasForNeuralNetwork(wayOpenLettMin, theta1LettMin, theta2LettMin);
    Matrix *ThetaOneLettMin = transpose(theta1LettMin);
    Matrix *ThetaTwoLettMin = transpose(theta2LettMin);
    destroyMatrix(theta1LettMin);
    destroyMatrix(theta2LettMin);
    
    Matrix *theta1LettMaj = constructeurMatrix(40, 785);
    Matrix *theta2LettMaj = constructeurMatrix(26, 41);
    RetrieveThetasForNeuralNetwork(wayOpenLettMaj, theta1LettMaj, theta2LettMaj);
    Matrix *ThetaOneLettMaj = transpose(theta1LettMaj);
    Matrix *ThetaTwoLettMaj = transpose(theta2LettMaj);
    destroyMatrix(theta1LettMaj);
    destroyMatrix(theta2LettMaj);

    char charac = _ApedemakNeuralNetwork(data, ThetaOneDigit, ThetaTwoDigit, ThetaOneLettMaj, ThetaTwoLettMaj, ThetaOneLettMin, ThetaTwoLettMin);
    FILE *file = fopen(wayToSave, "w");
    
    if (file == NULL){
        ErrorNotExist(wayToSave)
    }
    
    fprintf(file, "%c\n", charac);
    
    destroyMatrix(ThetaOneDigit);
    destroyMatrix(ThetaTwoDigit);
    destroyMatrix(ThetaOneLettMin);
    destroyMatrix(ThetaTwoLettMin);
    destroyMatrix(ThetaOneLettMaj);
    destroyMatrix(ThetaTwoLettMaj);
    
    fclose(file);
    
    return 1;
}


char _Apedemak(Matrix *prediction, Matrix *ThetaOne, Matrix *ThetaTwo, char type){
    Matrix *layer = sigmoidMatrix(MultiplyMatrix(prediction, ThetaOne));
    Matrix *hiddenLayer = addColumnOnesBeginMatrix(layer);
    destroyMatrix(layer);
    
    Matrix *outputLayer = sigmoidMatrix(MultiplyMatrix(hiddenLayer, ThetaTwo));
    destroyMatrix(hiddenLayer);
    
    int predic = 0;
    for (unsigned int i = 0; i < outputLayer->columns; i++) {
        if (outputLayer->matrice[0][predic] < outputLayer->matrice[0][i]){
            predic = i;
        }
    }
    
    destroyMatrix(outputLayer);
    
    if(type == '0'){
        return predic + 48;
    }
    if (type == '1') {
        return predic + 97;
    }
    if (type == '2'){
        return predic + 65;
    }
    
    if (predic >= 0 && predic <= 9)
        return predic + 48;
    if (predic >= 10 && predic <= 35) {
        return predic + 87;
    }
    
    return predic + 29;
}

/*Prediction*/
int ApedemakMain(struct par *prediction, char wayToSave[200], char wayToOpen[200], char type){
    // Type = 0 -> Digit
    // Type = 1 -> Lettre Min
    // Type = 2 -> Lettre Maj
    // Type = 3 -> Global
	
    if (prediction == NULL) {
        ErrorNotExist("Apedemak Main");
    }
    
    Matrix *TransTheta1 = NULL;
    Matrix *TransTheta2 = NULL;
    
    if(type == '0'){
        TransTheta1 = constructeurMatrix(30, 785);
        TransTheta2 = constructeurMatrix(10, 31);
        RetrieveThetasForNeuralNetwork(wayToOpen, TransTheta1, TransTheta2);
    }
	else if(type == '1'){
        TransTheta1 = constructeurMatrix(40, 785);
        TransTheta2 = constructeurMatrix(26, 41);
        RetrieveThetasForNeuralNetwork(wayToOpen, TransTheta1, TransTheta2);
    }
	else if(type == '2'){
        TransTheta1 = constructeurMatrix(40, 785);
        TransTheta2 = constructeurMatrix(26, 41);
        RetrieveThetasForNeuralNetwork(wayToOpen, TransTheta1, TransTheta2);
    }
	else if(type == '3'){
        TransTheta1 = constructeurMatrix(70, 785);
        TransTheta2 = constructeurMatrix(62, 71);
        RetrieveThetasForNeuralNetwork(wayToOpen, TransTheta1, TransTheta2);
    }
	else{
        return -1;
    }
    
    Matrix *ThetaOne = transpose(TransTheta1);
    Matrix *ThetaTwo = transpose(TransTheta2);

	FILE *file = fopen(wayToSave, "w");
	if (file == NULL){
		ErrorNotExist(wayToSave)
	}

	FILE *fichier = fopen("matrice", "w");

	while(prediction != NULL){
		struct line *line = prediction->lines;
		while(line != NULL){
			struct word *word = line->words;
			while(word != NULL){
				struct letter *letter = word->letters;
					while(letter != NULL){
						Matrix *matrix = letter->matrix;
                        Matrix *colonne = transformVectorColumnMatrix(matrix);
                        
                        for (unsigned int i = 1; i < colonne->columns; i++) {
                            int j = (int)colonne->matrice[0][i];
                            fprintf(fichier, "%d ", j);
                        }
                        
                        fprintf(fichier, "\n\n");

						char charac = _Apedemak(colonne, ThetaOne, ThetaTwo, type);
						fprintf(file, "%c", charac);
                        
						destroyMatrix(matrix);
						destroyMatrix(colonne);
						letter = letter->next;
					}
				if (word->next != NULL)	
					fprintf(file, " ");
				word = word->next;
			}
			if (line->next != NULL)
				fprintf(file, "\n");
			line = line->next;
		}
		fprintf(file, ".");
		fprintf(file, "\n\n");
		prediction = prediction->next;
	}
    
	fclose(fichier);
	fclose(file);
    
    destroyMatrix(ThetaOne);
    destroyMatrix(ThetaTwo);
    destroyMatrix(TransTheta1);
    destroyMatrix(TransTheta2);
    
    return 1;
}

