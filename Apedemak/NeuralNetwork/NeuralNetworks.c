//
//  NeuralNetworks.c
//  OCR
//
//  Created by Moustapha Diop on 22/11/2019.
//  Copyright © 2019 Moustapha Diop. All rights reserved.
//

#include "NeuralNetworks.h"

// For Neural Network Digit
#define NB_TRAINING_SET_DIGIT 1000
#define NB_TEST_SET_DIGIT 380

#define NB_UNITS_INPUT_LAYER_DIGIT 30
#define NB_UNITS_HIDDEN_LAYER_DIGIT 10

#define EPSILON_INIT_THETA1_DIGIT 0.09
#define EPSILON_INIT_THETA2_DIGIT 0.41

#define SAVE_THETAS_DIGIT "./NeuralNetwork/Data/ThetaDigitSoutenance"


// For Neural Network Letters Maj
#define NB_TRAINING_SET_LETTMAJ 1000
#define NB_TEST_SET_LETTMAJ 2000

#define NB_UNITS_INPUT_LAYER_LETTMAJ 40
#define NB_UNITS_HIDDEN_LAYER_LETTMAJ 26

#define EPSILON_INIT_THETA1_LETTMAJ 0.09
#define EPSILON_INIT_THETA2_LETTMAJ 0.41

#define SAVE_THETAS_LETTMAJ "./NeuralNetwork/Data/ThetaLettMajSoutenance"


// For Neural Network Letters Min
#define NB_TRAINING_SET_LETTMIN 1000
#define NB_TEST_SET_LETTMIN 2000

#define NB_UNITS_INPUT_LAYER_LETTMIN 40
#define NB_UNITS_HIDDEN_LAYER_LETTMIN 26

#define EPSILON_INIT_THETA1_LETTMIN 0.09
#define EPSILON_INIT_THETA2_LETTMIN 0.41

#define SAVE_THETAS_LETTMIN "./NeuralNetwork/Data/ThetaLettMinSoutenance"


// For Neural Network Apedemak
#define NB_TRAINING_SET_APEDEMAK 2000
#define NB_TEST_SET_APEDEMAK 2000

#define NB_UNITS_INPUT_LAYER_APEDEMAK 70
#define NB_UNITS_HIDDEN_LAYER_APEDEMAK 62

#define EPSILON_INIT_THETA1_APEDEMAK 0.29
#define EPSILON_INIT_THETA2_APEDEMAK 0.41

#define SAVE_THETAS_APEDEMAK "./NeuralNetwork/Data/ThetaApedemakSoutenance"

// Global Variable
#define PIXELS 784
#define MAX_ITERATION 100
#define PRECISION 0.0001


#define ErrorNotExist(name_file) \
printf("ErrorNotExist : This file '%s' does not exist. \n", name_file); \
exit(0);

#define ErrorMemory(memory) \
printf("Error Memory : Cannot access to this memory : This error comes from -> %s \n", memory); \
exit(0);


/*Train the Neural Network for Digit*/
void TrainNeuralNetworkDigit(char wayToLearn[200]){
    // INITIALISATION
    
    Matrix *XElements = constructeurMatrix(NB_TEST_SET_DIGIT + NB_TRAINING_SET_DIGIT, (int)PIXELS);
    Matrix *YElements = constructeurMatrix(NB_TEST_SET_DIGIT + NB_TRAINING_SET_DIGIT, 1);
    
    /*Update XElements and YElemets with the data*/
    parser(wayToLearn, XElements, YElements);
    
    Matrix *X = addColumnOnesBeginMatrix(XElements);
    destroyMatrix(XElements);
    
    /*Get the training and the test sets*/
    Matrix *XTrain = getRowsMatrix(X, 0, (int)NB_TRAINING_SET_DIGIT - 1);
    Matrix *XTest = getRowsMatrix(X, (int)NB_TRAINING_SET_DIGIT, (int)NB_TRAINING_SET_DIGIT + (int)NB_TEST_SET_DIGIT - 1);
    
    Matrix *YTrain = getRowsMatrix(YElements, 0, (int)NB_TRAINING_SET_DIGIT - 1);
    Matrix *YTest = getRowsMatrix(YElements, (int)NB_TRAINING_SET_DIGIT, (int)NB_TRAINING_SET_DIGIT + (int)NB_TEST_SET_DIGIT - 1);
    
    destroyMatrix(YElements);
    
    Matrix *thetaOne = NULL;
    Matrix *thetaTwo = NULL;
    
    double lambdas[12] = {3.5, 0.01, 0.02, 0.04, 0.08, 0.16, 0.32, 0.64, 1, 1.28, 2.56, 5.12};
    
    // END INITIALISATION
    
    for (unsigned i = 0; i < 1; i++) {
        //random initialisation of the thetas
        thetaOne = randomMatrix((int) NB_UNITS_INPUT_LAYER_DIGIT, XTrain->columns, (double) EPSILON_INIT_THETA1_DIGIT);
        thetaTwo = randomMatrix((int) NB_UNITS_HIDDEN_LAYER_DIGIT, thetaOne->rows + 1,(double) EPSILON_INIT_THETA2_DIGIT);
        
        //Train the Neural Network
        gradientDescent(thetaOne, thetaTwo, (int) MAX_ITERATION, XTrain, YTrain, lambdas[i], (double) PRECISION);
        
        printf("PrecisionTrain : %f pourcent \n", PresicionNeuralNetwork(thetaOne, thetaTwo, XTrain, YTrain));
        double precisionTest = PresicionNeuralNetwork(thetaOne, thetaTwo, XTest, YTest);
        printf("PrecisionTest : %f pourcent \n", precisionTest);
        
        SaveThetasOfNeuralNetwork(SAVE_THETAS_DIGIT, thetaOne, thetaTwo, (int)precisionTest, lambdas[i]);
        
        destroyMatrix(thetaOne);
        destroyMatrix(thetaTwo);
    }
    
    destroyMatrix(XTrain);
    destroyMatrix(XTest);
    destroyMatrix(YTrain);
    destroyMatrix(YTest);
    destroyMatrix(X);
}



/*Train the Neural Network for Letters Maj*/
void TrainNeuralNetworkLettersMaj(char wayToLearn[200]){
    // INITIALISATION
    
    Matrix *XElements = constructeurMatrix(NB_TEST_SET_LETTMAJ + NB_TRAINING_SET_LETTMAJ, (int)PIXELS);
    Matrix *YElements = constructeurMatrix(NB_TEST_SET_LETTMAJ + NB_TRAINING_SET_LETTMAJ, 1);
    
    /*Update XElements and YElemets with the data*/
    parser(wayToLearn, XElements, YElements);
    
    Matrix *X = addColumnOnesBeginMatrix(XElements);
    destroyMatrix(XElements);
    
    /*Get the training and the test sets*/
    Matrix *XTrain = getRowsMatrix(X, 0, (int)NB_TRAINING_SET_LETTMAJ - 1);
    Matrix *XTest = getRowsMatrix(X, (int)NB_TRAINING_SET_LETTMAJ, (int)NB_TRAINING_SET_LETTMAJ + (int)NB_TEST_SET_LETTMAJ - 1);
    
    Matrix *YTrain = getRowsMatrix(YElements, 0, (int)NB_TRAINING_SET_LETTMAJ - 1);
    Matrix *YTest = getRowsMatrix(YElements, (int)NB_TRAINING_SET_LETTMAJ, (int)NB_TRAINING_SET_LETTMAJ + (int)NB_TEST_SET_LETTMAJ - 1);
    
    destroyMatrix(YElements);
    
    Matrix *thetaOne = NULL;
    Matrix *thetaTwo = NULL;
    
    double lambdas[12] = {3, 0.01, 0.02, 0.04, 0.08, 0.16, 0.32, 0.64, 1, 1.28, 2.56, 5.12};
    
    // END INITIALISATION
    
    for (unsigned i = 0; i < 1; i++) {
        //random initialisation of the thetas
        thetaOne = randomMatrix((int) NB_UNITS_INPUT_LAYER_LETTMAJ, XTrain->columns, (double) EPSILON_INIT_THETA1_LETTMAJ);
        thetaTwo = randomMatrix((int) NB_UNITS_HIDDEN_LAYER_LETTMAJ, thetaOne->rows + 1,(double) EPSILON_INIT_THETA2_LETTMAJ);
        
        //Train the Neural Network
        gradientDescent(thetaOne, thetaTwo, (int) MAX_ITERATION, XTrain, YTrain, lambdas[i], (double) PRECISION);
        
        printf("PrecisionTrain : %f pourcent \n", PresicionNeuralNetwork(thetaOne, thetaTwo, XTrain, YTrain));
        double precisionTest = PresicionNeuralNetwork(thetaOne, thetaTwo, XTest, YTest);
        printf("PrecisionTest : %f pourcent \n", precisionTest);
        
        SaveThetasOfNeuralNetwork(SAVE_THETAS_LETTMAJ, thetaOne, thetaTwo, (int)precisionTest, lambdas[i]);
        
        
        destroyMatrix(thetaOne);
        destroyMatrix(thetaTwo);
    }
    
    destroyMatrix(XTrain);
    destroyMatrix(XTest);
    destroyMatrix(YTrain);
    destroyMatrix(YTest);
    destroyMatrix(X);
}



/*Train the Neural Network for letters Min*/
void TrainNeuralNetworkLettersMin(char wayToLearn[200]){
    // INITIALISATION
    
    Matrix *XElements = constructeurMatrix(NB_TEST_SET_LETTMIN + NB_TRAINING_SET_LETTMIN, (int)PIXELS);
    Matrix *YElements = constructeurMatrix(NB_TEST_SET_LETTMIN + NB_TRAINING_SET_LETTMIN, 1);
    
    /*Update XElements and YElemets with the data*/
    parser(wayToLearn, XElements, YElements);
    
    Matrix *X = addColumnOnesBeginMatrix(XElements);
    destroyMatrix(XElements);
    
    /*Get the training and the test sets*/
    Matrix *XTrain = getRowsMatrix(X, 0, (int)NB_TRAINING_SET_LETTMIN - 1);
    Matrix *XTest = getRowsMatrix(X, (int)NB_TRAINING_SET_LETTMIN, (int)NB_TRAINING_SET_LETTMIN + (int)NB_TEST_SET_LETTMIN - 1);
    
    Matrix *YTrain = getRowsMatrix(YElements, 0, (int)NB_TRAINING_SET_LETTMIN - 1);
    Matrix *YTest = getRowsMatrix(YElements, (int)NB_TRAINING_SET_LETTMIN, (int)NB_TRAINING_SET_LETTMIN + (int)NB_TEST_SET_LETTMIN - 1);
    
    destroyMatrix(YElements);
    
    Matrix *thetaOne = NULL;
    Matrix *thetaTwo = NULL;
    
    double lambdas[12] = {3, 0.01, 0.02, 0.04, 0.08, 0.16, 0.32, 0.64, 1, 1.28, 2.56, 5.12};
    
    // END INITIALISATION
    
    for (unsigned i = 0; i < 1; i++) {
        //random initialisation of the thetas
        thetaOne = randomMatrix((int) NB_UNITS_INPUT_LAYER_LETTMIN, XTrain->columns, (double) EPSILON_INIT_THETA1_LETTMIN);
        thetaTwo = randomMatrix((int) NB_UNITS_HIDDEN_LAYER_LETTMIN, thetaOne->rows + 1,(double) EPSILON_INIT_THETA2_LETTMIN);
        
        //Train the Neural Network
        gradientDescent(thetaOne, thetaTwo, (int) MAX_ITERATION, XTrain, YTrain, lambdas[i], (double) PRECISION);
        
        printf("PrecisionTrain : %f pourcent \n", PresicionNeuralNetwork(thetaOne, thetaTwo, XTrain, YTrain));
        double precisionTest = PresicionNeuralNetwork(thetaOne, thetaTwo, XTest, YTest);
        printf("PrecisionTest : %f pourcent \n", precisionTest);
        
        SaveThetasOfNeuralNetwork(SAVE_THETAS_LETTMIN, thetaOne, thetaTwo, (int)precisionTest, lambdas[i]);
        
        destroyMatrix(thetaOne);
        destroyMatrix(thetaTwo);
    }
    
    destroyMatrix(XTrain);
    destroyMatrix(XTest);
    destroyMatrix(YTrain);
    destroyMatrix(YTest);
    destroyMatrix(X);
}

/*Train all Neural Network*/
void TrainNeuralNetworkApedemak(char wayToLearn[200]){
    // INITIALISATION
    
    Matrix *XElements = constructeurMatrix(NB_TEST_SET_APEDEMAK + NB_TRAINING_SET_APEDEMAK, (int)PIXELS);
    Matrix *YElements = constructeurMatrix(NB_TEST_SET_APEDEMAK + NB_TRAINING_SET_APEDEMAK, 1);
    
    /*Update XElements and YElemets with the data*/
    parser(wayToLearn, XElements, YElements);
    
    Matrix *X = addColumnOnesBeginMatrix(XElements);
    destroyMatrix(XElements);
    
    /*Get the training and the test sets*/
    Matrix *XTrain = getRowsMatrix(X, 0, (int)NB_TRAINING_SET_APEDEMAK - 1);
    Matrix *XTest = getRowsMatrix(X, (int)NB_TRAINING_SET_APEDEMAK, (int)NB_TRAINING_SET_APEDEMAK + (int)NB_TEST_SET_APEDEMAK - 1);
    
    Matrix *YTrain = getRowsMatrix(YElements, 0, (int)NB_TRAINING_SET_APEDEMAK - 1);
    Matrix *YTest = getRowsMatrix(YElements, (int)NB_TRAINING_SET_APEDEMAK, (int)NB_TRAINING_SET_APEDEMAK + (int)NB_TEST_SET_APEDEMAK - 1);
    
    destroyMatrix(YElements);
    
    Matrix *thetaOne = NULL;
    Matrix *thetaTwo = NULL;
    
    double lambdas[12] = {3.5, 0.01, 0.02, 0.04, 0.08, 0.16, 0.32, 0.64, 1, 1.28, 2.56, 5.12};
    
    // END INITIALISATION
    
    for (unsigned i = 0; i < 1; i++) {
        //random initialisation of the thetas
        thetaOne = randomMatrix((int) NB_UNITS_INPUT_LAYER_APEDEMAK, XTrain->columns, (double) EPSILON_INIT_THETA1_APEDEMAK);
        thetaTwo = randomMatrix((int) NB_UNITS_HIDDEN_LAYER_APEDEMAK, thetaOne->rows + 1,(double) EPSILON_INIT_THETA2_APEDEMAK);
        
        //Train the Neural Network
        gradientDescent(thetaOne, thetaTwo, (int) MAX_ITERATION, XTrain, YTrain, lambdas[i], (double) PRECISION);
        
        printf("PrecisionTrain : %f pourcent \n", PresicionNeuralNetwork(thetaOne, thetaTwo, XTrain, YTrain));
        double precisionTest = PresicionNeuralNetwork(thetaOne, thetaTwo, XTest, YTest);
        printf("PrecisionTest : %f pourcent \n", precisionTest);
        
        SaveThetasOfNeuralNetwork(SAVE_THETAS_APEDEMAK, thetaOne, thetaTwo, (int)precisionTest, lambdas[i]);
        
        destroyMatrix(thetaOne);
        destroyMatrix(thetaTwo);
    }
    
    destroyMatrix(XTrain);
    destroyMatrix(XTest);
    destroyMatrix(YTrain);
    destroyMatrix(YTest);
    destroyMatrix(X);
}
