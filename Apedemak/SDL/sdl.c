#include <err.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "sdl.h"
#include <dirent.h>
#define GRAYIMAGE  "./data/gray.bmp"
#define IMAGE "./data/my_image4.jpg"
#define BLACKIMAGE "./data/black.bmp"
#define BLACKTEXT "./data/black.txt"
#define RESIZED "./data/resized.bmp"
#define DENOISED "./data/denoised.bmp"
#define DATA "./data/"
#pragma pack(1)  // unable memory alignement
typedef int int32;
typedef short int16;
int Showfolder(void)
{
   DIR * rep = opendir (DATA);
  
   if (rep != NULL)
   {
      struct dirent * ent;
  
      while ((ent = readdir (rep)) != NULL)
      {
          printf ("%s\n", ent->d_name);
      }
       
      closedir (rep);
   }
   return 0;
}

int Resize(SDL_Surface *img, int width, int height)
{
  SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
                        width,
                        height,
                        img->format->BitsPerPixel,0,0,0,0);
  SDL_SoftStretch(img, NULL, dest, NULL);
  Save(dest,RESIZED);
  return 0;
}

int Black_And_White(SDL_Surface *image_surface)
{
	Uint32 pixel;
	Uint8 r,g,b;
	Uint8 color;
	for (signed i = 0;i<image_surface->w;i++)
	{
		for (signed j = 0;j<image_surface->h;j++)
		{
			pixel = get_pixel(image_surface,i,j);
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			if (r>127)
				color=255;
			else
				color=0;
			r=color;
			g=color;
			b=color;
			pixel = SDL_MapRGB(image_surface->format, r, g, b);
			
			put_pixel(image_surface, i, j, pixel);
		}
	}
	Save(image_surface,BLACKIMAGE);
	SDL_FreeSurface(image_surface);
    return 0;
}

void Black_To_Text(SDL_Surface *image)
{
	Uint8 r,g,b;
	Uint32 p;
	FILE* F = fopen(BLACKTEXT,"wb");
	if (!F)
		return;
	char size[12] ;
	sprintf(size,"%d %d\n",image->w,image->h);
	fputs(size,F);
	for (int i = 0; i < image->h; ++i)
	{
		for (int j = 0; j < image->w; ++j)
		{
			p = get_pixel(image,j,image->h-i);
			SDL_GetRGB(p,image->format,&r,&g,&b);
			if (r==0)
			{
				fprintf(F, "1 ");
			}
			else
			{
                fprintf(F, "0 ");
			}
		}
	}
fclose(F);
SDL_FreeSurface(image);
}

void Transform(char *Image)
{	
	//Showfolder();
	SDL_Surface *I = load_image(Image);
	grayscale(I);
	printf("\n %s to gray done to : %s\n", Image, GRAYIMAGE);
	SDL_Surface *I2 = load_image(GRAYIMAGE);
	Black_And_White(I2);
	printf("\n %s to black and white done to : %s\n", GRAYIMAGE, BLACKIMAGE);
	SDL_Surface *I3=load_image(BLACKIMAGE);
	Black_To_Text(I3);
	printf("\n %s to text done to : %s\n", BLACKIMAGE, BLACKTEXT);
	SDL_Surface *I4=load_image(IMAGE);
	Resize(I4,28,28);
	printf("\n %s resized done to : %s\n", IMAGE, RESIZED);
}

SDL_Surface* NewImage(int w,int h)
{
	SDL_Surface *image = SDL_CreateRGBSurface(0,w,h,8,0,0,0,0);
	return image;
}
SDL_Surface* CopyImage(SDL_Surface *image)
{
	SDL_Surface *image2=SDL_CreateRGBSurface(0,image->w,image->h,8,image->format->Rmask,image->format->Gmask,image->format->Bmask,image->format->Amask);
	if (SDL_BlitSurface(image,NULL,image2,NULL)<0)
		errx(1,"Could not copy image :%s ",SDL_GetError());
	if (image2 == NULL)
		printf("image NULL");
	return image2;
}
struct BMPImHead
{
	int32 size_imhead;
	int32 width;
	int32 height;
	int16 nbplans;
	int16 bpp;
	int32 compression;
	int32 sizeim;
	int32 hres;
	int32 vres;
	int32 cpalette;
	int32 cIpalette;
};

struct BMPHead
{
	char sign[2];
	int32 size;
	int32 rsv;
	int32 offsetim;
	struct BMPImHead imhead;
};
int Save(SDL_Surface *surface,const char* file)
{
	return SDL_SaveBMP(surface, file);
}


Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        //errx(3, "can't load %s: %s", path, IMG_GetError());
		return NULL;
		
    return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

void SDL_FreeSurface(SDL_Surface *surface);
void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
    if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}

int cdf(int histo[],int coord)
{
	int count=0;
	for(int i=0;i<coord+1;i++)
		count+=histo[i];
	return count;
}
int* grayscale(SDL_Surface *image_surface)
{
	static int histo2[256];
	static int histo[256];
	for (int i =0;i<256;i++)
	{	
		histo[i]=0;
		histo2[i]=0;
	}
	Uint32 pixel;
	Uint8 r,g,b;
	Uint8 average;
	for (signed i = 0;i<image_surface->w;i++)
	{
		for (signed j = 0;j<image_surface->h;j++)
		{
			pixel = get_pixel(image_surface,i,j);
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			average = 0.3*r +0.59*g+0.11*b;
			r=average;
			g=average;
			b=average;
			pixel = SDL_MapRGB(image_surface->format, r, g, b);
			put_pixel(image_surface, i, j, pixel);
			histo[average]+=1;
		}
	}
	for (int i =0;i<256;i++)
	{
		histo2[i]=(int) (((float)(cdf(histo,i)-1)/(image_surface->h*image_surface->w))*255);
	}
	for (signed i = 0;i<image_surface->w;i++)
	{
		for (signed j = 0;j<image_surface->h;j++)
		{
			pixel = get_pixel(image_surface,i,j);
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			r=histo2[r];
			b=r;
			g=r;
			pixel = SDL_MapRGB(image_surface->format, r, g, b);
			put_pixel(image_surface, i, j, pixel);
		}
	}
	Save(image_surface,GRAYIMAGE);
	SDL_FreeSurface(image_surface);
    return histo2;
}
