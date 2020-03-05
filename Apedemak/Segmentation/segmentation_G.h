#ifndef SEGMENTATION_G_H
#define SEGMENTATION_G_H


#include "../NeuralNetwork/Matrix.h"
#include "../SDL/sdl.h"

struct par {
    SDL_Rect *rect;
    struct par *next;
    struct line *lines;
};

struct line {
    SDL_Rect *rect;
    struct line *next;
    struct word *words;
};

struct word {
    SDL_Rect *rect;
    struct word *next;
    struct letter *letters;
};

struct letter {
    SDL_Rect *rect;
    struct letter *next;
    Matrix *matrix;
};

struct par *Segmentation_G(SDL_Surface* I);
void Extract_all(SDL_Surface *I, SDL_Surface *Iresult, struct par *par);
struct par *Extract_par(SDL_Surface *I, SDL_Surface *Iresult);
int Get_last_char(SDL_Surface *I, int Xstart);
int Get_first_char(SDL_Surface *I, int Xstart);
int Get_first_col(SDL_Surface *I, int Ymin, int Ymax);
int Get_last_col(SDL_Surface *I, int Ymin, int Ymax);
unsigned int Get_first_line(SDL_Surface *I, int Ystart);
unsigned int Get_last_line(SDL_Surface *I, int Ystart);
void Segment(SDL_Surface* I, double Xpourcent, double Ypercent);
Matrix* binMatrix(SDL_Surface* I);
int GetCharCorners(Matrix* binarized, unsigned int row, unsigned int col, int *array);
void DrawLine(SDL_Surface *I, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);
void DrawSquare(SDL_Surface *I, int Xmin, int Xmax, int Ymin, int Ymax, Uint8 r, Uint8 g, Uint8 b);

#endif

