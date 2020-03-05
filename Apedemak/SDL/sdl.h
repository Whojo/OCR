#ifndef SDL_H_
#define SDL_H_

#include <stdlib.h>
#include <SDL/SDL.h>
int Denoise(SDL_Surface *image);
void bubbleSort(Uint32 arr[],int n );
void swap(Uint32 *xp,Uint32 *yp);
int Detect_Angle(SDL_Surface *image);
int Resized(SDL_Surface *Surface,Uint16 Width,Uint16 Height);
int Black_And_White(SDL_Surface *image);
void Black_To_Text(SDL_Surface *image);
void Transform();
SDL_Surface* NewImage(int w, int h);
SDL_Surface* CopyImage(SDL_Surface *image);
Uint8* pixel_ref(SDL_Surface *surf,unsigned x,unsigned y);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void init_sdl();
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
void wait_for_keypressed();
int* grayscale(SDL_Surface *image);
int Save(SDL_Surface *surface,const char* file);
void update_surface(SDL_Surface* screen, SDL_Surface* image);

#endif
