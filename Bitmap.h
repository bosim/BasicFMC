/// Handle cross platform differences
#if IBM
#include <gl\gl.h>
#include <gl\glu.h>
#elif LIN
#define TRUE 1
#define FALSE 0
#include <GL/gl.h>
#include <GL/glu.h>
#else
#define TRUE 1
#define FALSE 0
#if __GNUC__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <gl.h>
#include <glu.h>
#endif
#include <string.h>
#include <stdlib.h>
#endif

/// Texture stuff
#define MAX_TEXTURES 1
#define FMC_FILENAME			"fmc.bmp"
#define FMC_TEXTURE 0


/// Cross Platform Bitmap functions and bitmap data structures - Sandy Barbour 2003
/// These need to be aligned

#pragma pack(push, ident, 2)
typedef struct tagBMPFILEHEADER
{
    short  bfType;
    int	   bfSize;
    short  bfReserved1;
    short  bfReserved2;
    int    bfOffBits;
} BMPFILEHEADER;

typedef struct tagBMPINFOHEADER
{
   int     biSize;
   int     biWidth;
   int     biHeight;
   short   biPlanes;
   short   biBitCount;
   int     biCompression;
   int     biSizeImage;
   int     biXPelsPerMeter;
   int     biYPelsPerMeter;
   int     biClrUsed;
   int     biClrImportant;
} BMPINFOHEADER;

typedef struct	tagIMAGEDATA
{
	unsigned char *	pData;
	int			Width;
	int			Height;
	int			Padding;
	short		Channels;
} IMAGEDATA;
#pragma pack(pop, ident)

int BitmapLoader(const char *FilePath, IMAGEDATA *ImageData);
void SwapEndian(short *Data);
void SwapEndian(int *Data);
int LoadGLTexture(char *pFileName, int TextureId);
void LoadTextures(void);

#if APL && __MACH__
int ConvertPath(const char * inPath, char * outPath, int outPathMaxLen);
#endif
