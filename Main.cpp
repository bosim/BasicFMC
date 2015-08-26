/*
 * BasicFMC - A very basic FMC for X-Plane
 * Copyright (C) 2015 Bo Simonsen, <bo@geekworld.dk>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if IBM
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <iostream>
#include <string>

#include "XPLMDefs.h"
#include "XPLMDisplay.h"
#include "XPLMDataAccess.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"

#include "Bitmap.h"
#include "Pages.h"
#include "Page.h"
#include "Page_Init.h"
#include "Page_Legs.h"
#include "Flight.h"
#include "Main.h"

XPLMWindowID FMCWindow = NULL;
XPLMHotKeyID FMCToggleHotKey = NULL;
int FMCDisplayWindow = 0;

char PluginDir[255];
XPLMTextureID Texture[MAX_TEXTURES];

Pages* pages;
Flight* flight;

void InputHandler(int &dX, int &dY) {
  Page* page = pages->CurrentPage();
  
  if(CoordInRect(dX, dY, 152, -353, 180, -379)) {
    page->HandleInput('A');
  }
  else if(CoordInRect(dX, dY, 187, -353, 213, -379)) {
    page->HandleInput('B');
  }
  else if(CoordInRect(dX, dY, 221, -353, 245, -379)) {
    page->HandleInput('C');
  }
  else if(CoordInRect(dX, dY, 255, -353, 279, -379)) {
    page->HandleInput('D');
  }
  else if(CoordInRect(dX, dY, 288, -353, 314, -379)) {
    page->HandleInput('E');
  }
  else if(CoordInRect(dX, dY, 152, -388, 180, -411)) {
    page->HandleInput('F');
  }
  else if(CoordInRect(dX, dY, 187, -388, 213, -411)) {
    page->HandleInput('G');
  }
  else if(CoordInRect(dX, dY, 221, -388, 245, -411)) {
    page->HandleInput('H');
  }
  else if(CoordInRect(dX, dY, 255, -388, 279, -411)) {
    page->HandleInput('I');
  }
  else if(CoordInRect(dX, dY, 288, -388, 314, -411)) {
    page->HandleInput('J');
  }
  else if(CoordInRect(dX, dY, 152, -421, 180, -444)) {
    page->HandleInput('K');
  }
  else if(CoordInRect(dX, dY, 187, -421, 213, -444)) {
    page->HandleInput('L');
  }
  else if(CoordInRect(dX, dY, 221, -421, 245, -444)) {
    page->HandleInput('M');
  }
  else if(CoordInRect(dX, dY, 255, -421, 279, -444)) {
    page->HandleInput('N');
  }
  else if(CoordInRect(dX, dY, 288, -421, 314, -444)) {
    page->HandleInput('O');
  }
  else if(CoordInRect(dX, dY, 152, -455, 180, -477)) {
    page->HandleInput('P');
  }
  else if(CoordInRect(dX, dY, 187, -455, 213, -477)) {
    page->HandleInput('Q');
  }
  else if(CoordInRect(dX, dY, 221, -455, 245, -477)) {
    page->HandleInput('R');
  }
  else if(CoordInRect(dX, dY, 255, -455, 279, -477)) {
    page->HandleInput('S');
  }
  else if(CoordInRect(dX, dY, 288, -455, 314, -477)) {
    page->HandleInput('T');
  }
  else if(CoordInRect(dX, dY, 152, -489, 180, -512)) {
    page->HandleInput('U');
  }
  else if(CoordInRect(dX, dY, 187, -489, 213, -512)) {
    page->HandleInput('V');
  }
  else if(CoordInRect(dX, dY, 221, -489, 245, -512)) {
    page->HandleInput('W');
  }
  else if(CoordInRect(dX, dY, 255, -489, 279, -512)) {
    page->HandleInput('X');
  }
  else if(CoordInRect(dX, dY, 288, -489, 314, -512)) {
    page->HandleInput('Y');
  }
  else if(CoordInRect(dX, dY, 152, -523, 180, -547)) {
    page->HandleInput('Z');
  }
  else if(CoordInRect(dX, dY, 187, -523, 213, -547)) {
    page->HandleInput('-');
  }
  else if(CoordInRect(dX, dY, 221, -523, 245, -547)) {
    page->HandleInput('+');
  }
  else if(CoordInRect(dX, dY, 255, -523, 279, -547)) {
    page->HandleInput('*');
  }
  else if(CoordInRect(dX, dY, 288, -523, 314, -547)) {
    page->HandleDelete();
  }
  else if(CoordInRect(dX, dY, 41, -421, 72, -446)) {
    page->HandleInput('1');
  }
  else if(CoordInRect(dX, dY, 77, -421, 111, -446)) {
    page->HandleInput('2');
  }
  else if(CoordInRect(dX, dY, 114, -421, 146, -446)) {
    page->HandleInput('3');
  }
  else if(CoordInRect(dX, dY, 41, -452, 72, -479)) {
    page->HandleInput('4');
  }
  else if(CoordInRect(dX, dY, 77, -452, 111, -479)) {
    page->HandleInput('5');
  }
  else if(CoordInRect(dX, dY, 114, -452, 146, -479)) {
    page->HandleInput('6');
  }
  else if(CoordInRect(dX, dY, 41, -488, 72, -514)) {
    page->HandleInput('7');
  }
  else if(CoordInRect(dX, dY, 77, -488, 111, -514)) {
    page->HandleInput('8');
  }
  else if(CoordInRect(dX, dY, 114, -488, 146, -514)) {
    page->HandleInput('9');
  }
  else if(CoordInRect(dX, dY, 41, -521, 72, -547)) {
    page->HandleInput('.');
  }
  else if(CoordInRect(dX, dY, 77, -521, 111, -547)) {
    page->HandleInput('0');
  }
  else if(CoordInRect(dX, dY, 114, -521, 146, -547)) {
    page->HandleInput('/');
  }
  else if(CoordInRect(dX, dY, 186, -271, 222, -296)) {
    pages->SwitchPage("init");
  }
  else if(CoordInRect(dX, dY, 47, -307, 83, -333)) {
    pages->SwitchPage("legs");
  }
  else if(CoordInRect(dX, dY, 10, -71, 33, -87)) {
    page->HandleSK(LSK1);
  }
}

PLUGIN_API int XPluginStart(char * outName, char * outSig, char * outDesc) {

#if IBM
  const char* DirectoryName = "Resources\\Plugins\\BS-FMC\\";
#elif LIN
  const char* DirectoryName = "Resources/plugins/BS-FMC/";
#else
  const char* DirectoryName = "Resources:Plugins:BS-FMC:";
#endif

  XPLMGetSystemPath(PluginDir);
  strcat(PluginDir, DirectoryName);

  strcpy(outName, "BasicFMC");
  strcpy(outSig, "BasicFMC");
  strcpy(outDesc, "BasicFMC by Bo Simonsen.");

  int x = 0;
  int y = 720;

  int width = 360;
  int height = 570;

  FMCWindow = XPLMCreateWindow(x, y, x + width, y - height, 1,
                               FMCWindowCallback, FMCKeyCallback, 
                               FMCMouseClickCallback, NULL);

  FMCToggleHotKey = XPLMRegisterHotKey(XPLM_VK_F8, xplm_DownFlag,
                                       "F8", FMCToggleHotKeyHandler, NULL);

  LoadTextures();

  flight = new Flight();
  pages = new Pages();
        
  Page * page;
  
  page = new InitPage(flight);
  pages->RegisterPage("init", page);

  page = new LegsPage(flight);
  pages->RegisterPage("legs", page);

  pages->SwitchPage("init");
  
  return 1;
}

PLUGIN_API void	XPluginStop(void) {
  XPLMUnregisterHotKey(FMCToggleHotKey);
  XPLMDestroyWindow(FMCWindow);
        
  /* Cleaning up */
  auto iter_pair = pages->PagesIterator();
  
  for(auto iter = iter_pair.first; iter != iter_pair.second; iter++) {
    delete iter->second;
  }
  
  delete pages;
  delete flight;
}

PLUGIN_API void XPluginDisable(void) {

}

PLUGIN_API int XPluginEnable(void) {
  return 1;
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFromWho, int inMessage, void * inParam) {
  
}

void FMCWindowCallback(XPLMWindowID inWindowID, void * inRefcon) {
  int	PanelWindowLeft, PanelWindowRight, PanelWindowBottom, PanelWindowTop;
  float PanelLeft, PanelRight, PanelBottom, PanelTop;

  if (FMCDisplayWindow) {
    XPLMGetWindowGeometry(FMCWindow, &PanelWindowLeft, &PanelWindowTop, &PanelWindowRight, &PanelWindowBottom);
    PanelLeft = PanelWindowLeft; 
    PanelRight = PanelWindowRight; 
    PanelBottom = PanelWindowBottom; 
    PanelTop = PanelWindowTop;
    
    XPLMSetGraphicsState(0/*Fog*/, 1/*TexUnits*/, 0/*Lighting*/,
                         0/*AlphaTesting*/, 0/*AlphaBlending*/,
                         0/*DepthTesting*/, 0/*DepthWriting*/);
    
    XPLMBindTexture2d(Texture[FMC_TEXTURE], 0);
    
    glPushMatrix();
    glBegin(GL_QUADS);

    glTexCoord2f(1, 0.0f);
    glVertex2f(PanelRight, PanelBottom);
    // Bottom Right Of The Texture and Quad
    glTexCoord2f(0, 0.0f);
    glVertex2f(PanelLeft, PanelBottom);
    // Bottom Left Of The Texture and Quad
    glTexCoord2f(0, 1.0f);
    glVertex2f(PanelLeft, PanelTop);
    // Top Left Of The Texture and Quad
    glTexCoord2f(1, 1.0f);
    glVertex2f(PanelRight, PanelTop);
    // Top Right Of The Texture and Quad

    glEnd();
    glPopMatrix();

    XPLMSetGraphicsState(0/*Fog*/, 0/*TexUnits*/, 0/*Lighting*/,
                         0/*AlphaTesting*/, 0/*AlphaBlending*/,
                         0/*DepthTesting*/, 0/*DepthWriting*/);
    
    glFlush();

    Page * page = pages->CurrentPage();
    page->SetCoordinates(&PanelLeft, &PanelTop);
    page->Update();
  }
}

void FMCKeyCallback(XPLMWindowID inWindowID, char inKey, XPLMKeyFlags inFlags, char inVirtualKey, void * inRefcon, int losingFocus) {
}

int FMCMouseClickCallback(XPLMWindowID inWindowID, int x, int y, XPLMMouseStatus inMouse, void * inRefcon) {
  static int dX = 0, dY = 0;
  static int Weight = 0, Height = 0;
  int Left, Top, Right, Bottom;
  static int gDragging = 0;

  if (!FMCDisplayWindow)
    return 0;
        
  /// Get the windows current position
  XPLMGetWindowGeometry(inWindowID, &Left, &Top, &Right, &Bottom);

  switch(inMouse) {
  case xplm_MouseDown:
    dX = x - Left;
    dY = y - Top;
    /// Test for the mouse in the top part of the window
    if (CoordInRect(x, y, Left, Top, Right, Top-15)) {
      Weight = Right - Left;
      Height = Bottom - Top;
      gDragging = 1;
    }
    else {
      InputHandler(dX, dY);
    }
    break;
  case xplm_MouseDrag:
    /// We are dragging so update the window position
    if (gDragging) {
      Left = (x - dX);
      Right = Left + Weight;
      Top = (y - dY);
      Bottom = Top + Height;
      XPLMSetWindowGeometry(inWindowID, Left, Top, Right, Bottom);
    }
    break;
  case xplm_MouseUp:
    gDragging = 0;
    break;
  }

  return 1;
}

void FMCToggleHotKeyHandler(void * refCon) {
  FMCDisplayWindow = !FMCDisplayWindow;
}

int CoordInRect(float x, float y, float l, float t, float r, float b) {
  return ((x >= l) && (x < r) && (y < t) && (y >= b));
}




