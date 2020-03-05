#include "segmentation_G.h"
#include <stdio.h>
#include <stdlib.h>

static inline int min(int i1, int i2){
    if (i1 < i2)
        return i1;
    return i2;
}

static inline int max(int i1, int i2){
    if (i1 > i2)
        return i1;
    return i2;
}


struct par *Segmentation_G(SDL_Surface* I){
    SDL_Surface *Iresult = SDL_CreateRGBSurface(0,I->w,I->h,32,0,0,0,0);
    SDL_BlitSurface(I, NULL, Iresult, NULL);
    SDL_Surface *Ipar = SDL_CreateRGBSurface(0,I->w,I->h,32,0,0,0,0);
    SDL_BlitSurface(I, NULL, Ipar, NULL);
    SDL_Surface *Iline = SDL_CreateRGBSurface(0,I->w,I->h,32,0,0,0,0);
    SDL_BlitSurface(I, NULL, Iline, NULL);

    Segment(Ipar, 0.8, 0.8);
    struct par *par = Extract_par(Ipar, Iresult);

    Extract_all(Iline, Iresult, par);
    Save(Iresult, "./data/result.bmp");

    return par;
}

SDL_Surface *_Resize(SDL_Surface *img, int width, int height)
{
    SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
                                             width,
                                             height,
                                             img->format->BitsPerPixel,0,0,0,0);
    SDL_SoftStretch(img, NULL, dest, NULL);
    
    return dest;
}

void Extract_all(SDL_Surface *I, SDL_Surface *Iresult, struct par *par){
    // Exract Lines
    for (struct par *p = par; p != NULL; p = p->next){
        SDL_Surface *Il = SDL_CreateRGBSurface(0,p->rect->w,p->rect->h,32,0,0,0,0);
        SDL_BlitSurface(I, p->rect, Il, NULL);
        SDL_Surface *Il2 = SDL_CreateRGBSurface(0,p->rect->w,p->rect->h,32,0,0,0,0);
        SDL_BlitSurface(I, p->rect, Il2, NULL);
        Segment(Il, 0.4, 0.1);

        struct line *lines = NULL;
        struct line *next = NULL;

        for (int Ymax = Get_last_line(Il, Il->h - 1); Ymax > 0; Ymax--) {
            int Ymin = Get_first_line(Il, Ymax);
            DrawSquare(Iresult, p->rect->x + 1, p->rect->x + p->rect->w - 1, p->rect->y + Ymin, p->rect->y + Ymax, 127, 255, 0);

            lines = malloc(sizeof(struct line));
            lines->rect = malloc(sizeof(SDL_Rect));
            lines->rect->x = 0;
            lines->rect->y = Ymin;
            lines->rect->w = p->rect->w;
            lines->rect->h = Ymax - Ymin;

            lines->next = next;
            next = lines;

            Ymax = Get_last_line(Il, Ymin);
        }

        p->lines = lines;


        // Extract words
        for (struct line *l = lines; l != NULL; l = l->next){
            SDL_Surface *Iw = SDL_CreateRGBSurface(0,l->rect->w,l->rect->h,32,0,0,0,0);
            SDL_BlitSurface(Il2, l->rect, Iw, NULL);
            SDL_Surface *I3 = SDL_CreateRGBSurface(0,l->rect->w,l->rect->h,32,0,0,0,0);
            SDL_BlitSurface(Iw, NULL, I3, NULL);
            Segment(Iw, 0.4, 0.1);

            struct word *words = NULL;
            struct word *next = NULL;

            for (int Xmax = Get_last_char(Iw, Iw->w - 1); Xmax > 0; Xmax--) {
                int Xmin = Get_first_char(Iw, Xmax);
                DrawSquare(Iresult, p->rect->x + l->rect->x + Xmin, p->rect->x + l->rect->x + Xmax, p->rect->y + l->rect->y + 1, p->rect->y + l->rect->y + l->rect->h - 1, 0, 0, 255);

                words = malloc(sizeof(struct word));
                words->rect = malloc(sizeof(SDL_Rect));
                words->rect->x = Xmin;
                words->rect->y = 0;
                words->rect->w = Xmax - Xmin;
                words->rect->h = l->rect->h;

                words->next = next;
                next = words;

                Xmax = Get_last_char(Iw, Xmin);
            }

            l->words = words;


            // Extract letters
            for (struct word *w = words; w != NULL; w = w->next){
                SDL_Surface *Ile = SDL_CreateRGBSurface(0,w->rect->w,w->rect->h,32,0,0,0,0);
                SDL_BlitSurface(I3, w->rect, Ile, NULL);

                // Ici pour modifier les tailles
                double Xpourcent = 0.17;
                double Ypourcent = 0;

                struct letter *letters = NULL;
                struct letter *fletters = NULL;
                struct letter *previous = NULL;
                int first = 1;

                Matrix* binarized = binMatrix(Ile);
                Matrix* isTested = constructeurMatrix(Ile->h, Ile->w);

                int isLetter = 0;
                int Xmax = 0;
                int Xmin = binarized->columns;
                int Ymax = 0;
                int Ymin = binarized->rows;

                for (unsigned int col = 1; col < binarized->columns - 1; col++){
                    for (unsigned int row = 1; row < binarized->rows - 1; row++){
                        if(binarized->matrice[row][col] && !isTested->matrice[row][col]){
                            int array[4];
                            if(GetCharCorners(binarized, row, col, array)){
                                int *corners = array;
                                int currentXmin = *(corners); //X = cols
                                int currentXmax = *(corners + 1);
                                int currentYmin = *(corners + 2); //Y = rows
                                int currentYmax = *(corners + 3);

                                Xmin = min(Xmin, currentXmin);
                                Xmax = max(Xmax, currentXmax);
                                Ymin = min(Ymin, currentYmin);
                                Ymax = max(Ymax, currentYmax);

                                isLetter = 1;
                            }
                        }
                    }
                    if (isLetter){
                        int height = Ymax - Ymin;
                        Xmin -= Xpourcent * height;
                        Ymin -= Ypourcent * height;
                        Xmax += Xpourcent * height;
                        Ymax += Ypourcent * height;

                        for (int x = Xmin; x < Xmax; ++x) {
                            for (int y = Ymin; y < Ymax; ++y)
                                isTested->matrice[y][x] = 1;
                        }
                        col = Xmin - 1;

                        DrawSquare(Iresult, p->rect->x + l->rect->x + w->rect->x + Xmin, p->rect->x + l->rect->x + w->rect->x + Xmax, p->rect->y + l->rect->y + w->rect->y + Ymin, p->rect->y + l->rect->y + w->rect->y + Ymax, 127, 127, 0);

                        letters = malloc(sizeof(struct letter));
                        letters->rect = malloc(sizeof(SDL_Rect));
                        letters->rect->x = Xmin;
                        letters->rect->y = Ymin;
                        letters->rect->w = Xmax - Xmin;
                        letters->rect->h = Ymax - Ymin;
                        if (first)
                            fletters = letters;

                        SDL_Surface *Iletter = SDL_CreateRGBSurface(0,letters->rect->w,letters->rect->h,32,0,0,0,0);
                        SDL_BlitSurface(Ile, letters->rect, Iletter, NULL);
                                
                        //resize 28 28
                        Iletter = _Resize(Iletter, 28, 28);
                        letters->matrix = binMatrix(Iletter);

                        if (!first)
                            previous->next = letters;
                        else
                            first = 0;
                        previous = letters;
                        letters->next = NULL;

                        isLetter = 0;
                        Xmax = 0;
                        Ymax = 0;
                        Xmin = binarized->columns;
                        Ymin = binarized->rows;
                    }
                        //}
                    //}
                }
                w->letters = fletters;
            }
        }
    }
}


struct par *Extract_par(SDL_Surface *I, SDL_Surface *Iresult){
    struct par *par = NULL;
    struct par *next = NULL;

    for (int Ymax = Get_last_line(I, I->h - 5); Ymax > 0; Ymax--) {
        int Ymin = Get_first_line(I, Ymax - 1);
        int Xmin = Get_first_col(I, Ymin, Ymax);
        int Xmax = Get_last_col(I, Ymin, Ymax);

        DrawSquare(Iresult, Xmin, Xmax, Ymin, Ymax, 255, 0, 0);

        par = malloc(sizeof(struct par));
        par->rect = malloc(sizeof(SDL_Rect));
        par->rect->x = Xmin;
        par->rect->y = Ymin;
        par->rect->w = Xmax - Xmin;
        par->rect->h = Ymax - Ymin;

        par->next = next;
        next = par;

        Ymax = Get_last_line(I, Ymin);
    }

    return par;
}


int Get_last_char(SDL_Surface *I, int Xstart){
    Matrix* binarized = binMatrix(I);

    unsigned int x;
    unsigned int y = binarized->rows;
    for (x = Xstart; x > 0 && y >= binarized->rows; x--)
        for (y = 0; y < binarized->rows && !binarized->matrice[y][x]; y++);

    return x + 1;
}


int Get_first_char(SDL_Surface *I, int Xstart){
    Matrix* binarized = binMatrix(I);

    unsigned int x;
    unsigned int y = 0;
    for (x = Xstart; x > 0 && y < binarized->rows; x--)
        for (y = 0; y < binarized->rows && !binarized->matrice[y][x]; y++);

    return x + 1;
}


int Get_first_col(SDL_Surface *I, int Ymin, int Ymax){
    Matrix* binarized = binMatrix(I);
    unsigned int Xmin = binarized->columns;

    for (int y = Ymin; y < Ymax; y++){
        unsigned int x = 0;
        for (; x < binarized->columns && !binarized->matrice[y][x]; x++);
        Xmin = min(Xmin, x);
    }

    return Xmin - 1;
}


int Get_last_col(SDL_Surface *I, int Ymin, int Ymax){
    Matrix* binarized = binMatrix(I);
    int Xmax = 0;

    for (int y = Ymin; y < Ymax; y++){
        int x = binarized->columns - 1;
        for (; x > 0 && !binarized->matrice[y][x]; x--);
        Xmax = max(Xmax, x);
    }

    return Xmax + 1;
}


unsigned int Get_first_line(SDL_Surface *I, int Ystart){
    Matrix* binarized = binMatrix(I);

    int y;
    unsigned int x = 0;
    for (y = Ystart; y >= 0 && x < binarized->columns; y--)
        for (x = 0; x < binarized->columns && !binarized->matrice[y][x]; x++);

    return y + 1;
}


unsigned int Get_last_line(SDL_Surface *I, int Ystart){
    Matrix* binarized = binMatrix(I);

    unsigned int y;
    unsigned int x = binarized->columns;
    for (y = Ystart; y > 0 && x >= binarized->columns; y--)
        for (x = 0; x < binarized->columns && !binarized->matrice[y][x]; x++);

    return y + 1;
}


void Segment(SDL_Surface* I, double Xpourcent, double Ypourcent){
    Matrix* binarized = binMatrix(I);
    Matrix* isTested = constructeurMatrix(I->h, I->w); //Copy the dimension of binarized and fill it with 0
    
    for (unsigned int row = 1; row < binarized->rows - 5; row++){
        for (unsigned int col = 1; col < binarized->columns - 1; col++){
            //printf("%u x %u :\n", row, col);
            //printf("%f \n", binarized->matrice[row][col]);
            //printf("%f \n", isTested->matrice[row][col]);
            if(binarized->matrice[row][col] && !isTested->matrice[row][col]){ //Process only in case that there is a black pixel and this one has never been tested
                int array[4];
                if(GetCharCorners(binarized, row, col, array)){
                    int *corners = array;
                    int Xmin = *(corners); //X = cols
                    int Xmax = *(corners + 1);
                    int Ymin = *(corners + 2); //Y = rows
                    int Ymax = *(corners + 3);

                    for (int x = Xmin; x < Xmax; ++x) {
                        for (int y = Ymin; y < Ymax; ++y)
                            isTested->matrice[y][x] = 1;
                    }
                    col = Xmax;

                    int height = Ymax - Ymin;
                    DrawSquare(I, Xmin - Xpourcent * height, Xmax + Xpourcent * height, Ymin - Ypourcent * height, Ymax + Ypourcent * height, 255, 0, 0);
                }
            }
        }
    }
}


Matrix* binMatrix(SDL_Surface* I){
    Uint8 r,g,b;
    Matrix* binarized = constructeurMatrix(I->h, I->w);
    for(unsigned int y = 0;y < binarized->rows;y++) {
        for(unsigned int x = 0;x < binarized->columns;x++) {
            Uint32 curr = get_pixel(I, x, y);
            SDL_GetRGB(curr, I->format, &r, &g, &b);
            if(b == 0) 
                binarized->matrice[y][x] = 1;
            else
                binarized->matrice[y][x] = 0;
        }
    }
    return binarized;
}


int GetCharCorners(Matrix* binarized, unsigned int row, unsigned int col, int *array){
    int Xmin = binarized->columns, Xmax = 0, Ymin = binarized->rows, Ymax = 0;
    unsigned int x = col - 1, y = row, i = 0;
    int nextPoint[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}; // {up, right, down, left}

    do {
        if (binarized->matrice[y + nextPoint[i][1]][x + nextPoint[i][0]] || ! binarized->matrice[y + nextPoint[(i + 2)  % 4][1]][x + nextPoint[(i + 2) % 4][0]]){
            if (binarized->matrice[y + nextPoint[i][1]][x + nextPoint[i][0]] && !(binarized->matrice[y + nextPoint[(i + 3) % 4][1]][x + nextPoint[(i + 3) % 4][0]]))
                i = (i + 3) % 4;
            else if (binarized->matrice[y + nextPoint[(i + 3) % 4][1]][x + nextPoint[(i + 3) % 4][0]] && !(binarized->matrice[y + nextPoint[(i + 2) % 4][1]][x + nextPoint[(i + 2) % 4][0]]))
                i = (i + 2) % 4;
            else if (binarized->matrice[y + nextPoint[(i + 2) % 4][1]][x + nextPoint[(i + 2) % 4][0]] && !(binarized->matrice[y + nextPoint[(i + 1) % 4][1]][x + nextPoint[(i + 1) % 4][0]]))
                i = (i + 1) % 4;
        }

        x += nextPoint[i][0];
        y += nextPoint[i][1];
        i = (i + 1) % 4;

        Xmin = min(x, Xmin);
        Xmax = max(x, Xmax);
        Ymin = min(y, Ymin);
        Ymax = max(y, Ymax);

        if (x >= binarized->columns || y >= binarized->rows)
            return 0;

    } while (x != col - 1 || y != row);
    
    int arr[4] = {Xmin + 1, Xmax, Ymin + 1, Ymax};
    for (int i = 0; i != 4; i++){
        array[i] = arr[i];
    }
    return 1;
}


void DrawLine(SDL_Surface *I, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b){
    x1 = max(min(x1, I->w - 1), 1);
    y1 = max(min(y1, I->h - 1), 1);
    x2 = max(min(x2, I->w - 1), 1);
    y2 = max(min(y2, I->h - 1), 1);

    if (x1 == x2 && y1 == y2)
        put_pixel(I, x1, y1, SDL_MapRGB(I->format, r, g, b));
    
    else if (x1 == x2) {
        for (int y = min(y1, y2); y <= max(y1, y2); ++y) {
            put_pixel(I, x1, y, SDL_MapRGB(I->format, r, g, b));
        }
    }
    else if (abs(x1 - x2) > abs(y1 - y2)) {
        double delta = (y1 - y2) / (double) (x1 - x2);
        
        for (int x = min(x1, x2); x <= max(x1, x2); x++) {
            int y = delta * (x - x1) + y1;
            put_pixel(I, x, y, SDL_MapRGB(I->format, r, g, b));
        }
    }
    else {
        double delta = (x1 - x2) / (double) (y1 - y2);

        for (int y = min(y1, y2); y <= max(y1, y2); y++) {
            int x = delta * (y - y1) + x1;
            put_pixel(I, x, y, SDL_MapRGB(I->format, r, g, b));
        }
    }
}


void DrawSquare(SDL_Surface *I, int Xmin, int Xmax, int Ymin, int Ymax, Uint8 r, Uint8 g, Uint8 b){
    DrawLine(I, Xmin, Ymax, Xmax, Ymax, r, g, b);
    DrawLine(I, Xmax, Ymax, Xmax, Ymin, r, g, b);
    DrawLine(I, Xmax, Ymin, Xmin, Ymin, r, g, b);
    DrawLine(I, Xmin, Ymin, Xmin, Ymax, r, g, b);
}

