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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include "Bitmap.h"

#include "XPLMDefs.h"
#include "XPLMDisplay.h"
#include "XPLMDataAccess.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"

extern XPLMTextureID Texture[MAX_TEXTURES];
extern char PluginDir[255];

/// Loads all our textures
void LoadTextures(void) {
	if (!LoadGLTexture(FMC_FILENAME, FMC_TEXTURE))
		XPLMDebugString("Panel texture failed to load\n");
}

/// Loads one texture
int LoadGLTexture(char *pFileName, int TextureId)
{
  int Status=FALSE;
  char TextureFileName[255];
#if APL && __MACH__
  char TextureFileName2[255];
  int Result = 0;
#endif

  /// Need to get the actual texture path
  /// and append the filename to it.
  strcpy(TextureFileName, PluginDir);
  strcat(TextureFileName, pFileName);
  
#if APL && __MACH__
  Result = ConvertPath(TextureFileName, TextureFileName2, sizeof(TextureFileName));
  if (Result == 0)
    strcpy(TextureFileName, TextureFileName2);
  else
    XPLMDebugString("ExampleGauge - Unable to convert path\n");
#endif

  void *pImageData = 0;
  int sWidth, sHeight;
  IMAGEDATA sImageData;
  /// Get the bitmap from the file
  if (BitmapLoader(TextureFileName, &sImageData)) {
    Status=TRUE;

    SwapRedBlue(&sImageData);
    pImageData = sImageData.pData;

    /// Do the opengl stuff using XPLM functions for a friendly Xplane existence.
    sWidth=sImageData.Width;
    sHeight=sImageData.Height;
    XPLMGenerateTextureNumbers(&Texture[TextureId], 1);
    XPLMBindTexture2d(Texture[TextureId], 0);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, sWidth, sHeight, GL_RGB, GL_UNSIGNED_BYTE, pImageData);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  }
  if (pImageData != NULL)
    free(pImageData);
  
  return Status;
}



#if APL && __MACH__
#include <Carbon/Carbon.h>
int ConvertPath(const char * inPath, char * outPath, int outPathMaxLen) {
  CFStringRef inStr = CFStringCreateWithCString(kCFAllocatorDefault, inPath ,kCFStringEncodingMacRoman);
  if (inStr == NULL)
    return -1;
  CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, inStr, kCFURLHFSPathStyle,0);
  CFStringRef outStr = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
  if (!CFStringGetCString(outStr, outPath, outPathMaxLen, kCFURLPOSIXPathStyle))
    return -1;
  CFRelease(outStr);
  CFRelease(url);
  CFRelease(inStr);
  return 0;
}
#endif


/// Cross Platform Bitmap functions - Sandy Barbour 2003
/// Functions to handle endian differeneces between windows, linux and mac.
#if APL
short Endian(short Data) {
  unsigned char *pBuffer = (unsigned char *)&Data;
  short Result = (short)(pBuffer[0] & 0xff) + ( (short)(pBuffer[1] & 0xff) << 8) ;
  return(Result);
}

int Endian(int Data) {
  unsigned char *pBuffer = (unsigned char *)&Data;
  int Result = 		(int)(pBuffer[0] & 0xff)
    + ( (int)(pBuffer[1] & 0xff) << 8)
    + ( (int)(pBuffer[2] & 0xff) << 16)
    + ( (int)(pBuffer[3] & 0xff) << 24);
  
  return(Result);
}

void SwapEndian(short *Data) {
  *Data = Endian(*Data);
}

void SwapEndian(int *Data) {
  *Data = Endian(*Data);
}
#else
/// Only the mac needs these so dummy functions for windows and linux.
void SwapEndian(short *Data){}
void SwapEndian(int *Data){}
#endif

/// Swap the red and blue pixels.
void SwapRedBlue(IMAGEDATA *ImageData)
{
	unsigned char  * srcPixel;
	int 	count;
	int		x,y;
	unsigned char sTemp;

	/// Does not support 4 channels.
	if (ImageData->Channels == 4)
		return;

	/// Do the swap
	srcPixel = ImageData->pData;
	count = ImageData->Width * ImageData->Height;
	for (y = 0; y < ImageData->Height; ++y)
		for (x = 0; x < ImageData->Width; ++x)
		{
			sTemp = srcPixel[0];
			srcPixel[0] = srcPixel[2];
			srcPixel[2] = sTemp;

			srcPixel += 3;
			if (x == (ImageData->Width - 1))
				srcPixel += ImageData->Padding;
		}
}


/// Generic bitmap loader to handle all platforms
int BitmapLoader(const char * FilePath, IMAGEDATA * ImageData) {
  BMPFILEHEADER   Header;
  BMPINFOHEADER	ImageInfo;
  int						Padding;
  FILE *					BitmapFile = NULL;
  int RetCode = 0;

  ImageData->pData = NULL;

  BitmapFile = fopen(FilePath, "rb");
  if (BitmapFile != NULL) {
    if (fread(&Header, sizeof(Header), 1, BitmapFile) == 1) {
      if (fread(&ImageInfo, sizeof(ImageInfo), 1, BitmapFile) == 1) {
        /// Handle Header endian.
        SwapEndian(&Header.bfSize);
        SwapEndian(&Header.bfOffBits);
        
        /// Handle ImageInfo endian.
        SwapEndian(&ImageInfo.biWidth);
        SwapEndian(&ImageInfo.biHeight);
        SwapEndian(&ImageInfo.biBitCount);

        /// Make sure that it is a bitmap.
#if APL && defined(__POWERPC__)
        if (((Header.bfType & 0xff) == 'M') &&
            (((Header.bfType >> 8) & 0xff) == 'B') &&
#else
        if (((Header.bfType & 0xff) == 'B') &&
            (((Header.bfType >> 8) & 0xff) == 'M') &&
#endif
            (ImageInfo.biBitCount == 24) &&
            (ImageInfo.biWidth > 0) &&
            (ImageInfo.biHeight > 0)) {
          /// Sandy Barbour - I have found that "Header.bfSize" does not always agree
          /// with the actual file size and can sometimes be "ImageInfo.biSize"	 smaller.
          /// So add it in for good measure
          if ((Header.bfSize + ImageInfo.biSize - Header.bfOffBits) >= (ImageInfo.biWidth * ImageInfo.biHeight * 3)) {
            Padding = (ImageInfo.biWidth * 3 + 3) & ~3;
            Padding -= ImageInfo.biWidth * 3;
            ImageData->Width = ImageInfo.biWidth;
            ImageData->Height = ImageInfo.biHeight;
            ImageData->Padding = Padding;

            /// Allocate memory for the actual image.
            ImageData->Channels = 3;
            ImageData->pData = (unsigned char *) malloc(ImageInfo.biWidth * ImageInfo.biHeight * ImageData->Channels + ImageInfo.biHeight * Padding);

            if (ImageData->pData != NULL) {
              /// Get the actual image.
              if (fread(ImageData->pData, ImageInfo.biWidth * ImageInfo.biHeight * ImageData->Channels + ImageInfo.biHeight * Padding, 1, BitmapFile) == 1) {
                RetCode = 1;
              }
            }
          }
        }
      }
    }
  }
  if (BitmapFile != NULL)
    fclose(BitmapFile);
  return RetCode;
}
