/* Copyright (c) 2008, Sandy Barbour and Ben Supnik
All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Neither the names of the authors nor that of X-Plane or Laminar Research
      may be used to endorse or promote products derived from this software 
      without specific prior written permission from the authors or 
      Laminar Research, respectively.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
void SwapRedBlue(IMAGEDATA *ImageData);
int LoadGLTexture(std::string, int TextureId);
void LoadTextures(void);

#if APL && __MACH__
int ConvertPath(const char * inPath, char * outPath, int outPathMaxLen);
#endif
