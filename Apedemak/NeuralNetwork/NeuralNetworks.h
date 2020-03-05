//
//  NeuralNetworks.h
//  OCR
//
//  Created by Moustapha Diop on 22/11/2019.
//  Copyright © 2019 Moustapha Diop. All rights reserved.
//

#ifndef NeuralNetworks_h
#define NeuralNetworks_h

#include <stdio.h>

#include "Matrix.h"
#include "Functions.h"
#include "AnnexesNeuralNetworks.h"

/*Train the Neural Network for Digit*/
void TrainNeuralNetworkDigit(char wayToLearn[200]);

/*Train the Neural Network for Letters Maj*/
void TrainNeuralNetworkLettersMaj(char wayToLearn[200]);

/*Train the Neural Network for letters Min*/
void TrainNeuralNetworkLettersMin(char wayToLearn[200]);

/*Train all Neural Network*/
void TrainNeuralNetworkApedemak(char wayToLearnDigit[200]);

#endif /* NeuralNetworks_h */
