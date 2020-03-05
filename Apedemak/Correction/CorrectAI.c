#include "ctype.h"
#include <stdio.h>
#include "CorrectAI.h"
#include <string.h>


void correct(){
    FILE *output = fopen("./data/output.txt", "r");
    FILE *newOutput = fopen("./data/newOutput.txt", "w");
    FILE *dico = fopen("./Correction/FrenchWords.dic", "r");

    char Dword[50];
    char word[50];
    int i = 0;
    char c;

    do {
        c = fgetc(output);
        if (c != ' ' && c != '\n' && c != EOF) {
            word[i] = c;
            i += 1;
        }

        else if (c == ' ' || c == '\n') {
            word[i] = 0;

            // Find Best word correspondance in the dico
            double bestScore = 0;
            char best[50] = "";

            do {
                fgets(Dword, 50, dico);

                // Remove last char
                int i = 0;
                for (; Dword[i + 1] != 0; i++);
                Dword[i] = 0;

                double score = distance(word, Dword);
                if (score > bestScore){
                    bestScore = score;
                    strcpy(best, Dword);
                }
            } while (Dword[0] != 0 && bestScore != 100);
            rewind(dico);

            // Write Best word in the new file
            fputs(best, newOutput);
            if (c == ' ')
                fputs(" ", newOutput);
            else if (c == '\n')
                fputs("\n", newOutput);
            i = 0;
        }

    } while (c != EOF);

    fclose(dico);
    fclose(output);
    fclose(newOutput);
}


double distance(char *w1, char *w2){
    //w2 == word from dictinnary

    //m = 7
    //j = ja
    //l = 1
    //Minuscule == majuscule

    int length = 0;
    double correct = 0;

    int i = 0, j = 0;
    for (; w1[i] != 0 && w2[j] != 0; i++, j++){
        if (tolower(w1[i]) == 'j'){
            correct += tolower(w1[i]) == tolower(w2[j]);
            i += 1;
        }
        else if (w1[i] == w2[j])
            correct += 1;
        else if (tolower(w1[i]) == tolower(w2[j]))
            correct += 0.8;
        else if (tolower(w1[i]) == '7' || tolower(w1[i] == 'T'))
            correct += tolower(w2[j]) == 'm';
        else if (w1[i] == '0')
            correct += tolower(w2[j]) == 'o' || w2[j] == 'O';
        else if (tolower(w1[i]) == '1')
            correct += tolower(w2[j]) == 'l' || tolower(w2[j]) == 'i' || w2[j] == 'I';

        length += 1;
    }

    for (;w1[i] != 0; i++, length++);
    for (;w2[j] != 0; j++, length++);

    return correct * 100 / length;
}
