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
#include "XPLMPlugin.h"
#include "XPLMProcessing.h"

#include "Bitmap.h"
#include "Pages.h"
#include "Page.h"
#include "Page_Init.h"
#include "Page_Legs.h"
#include "Page_Airport.h"
#include "Page_Progress.h"
#include "Flight.h"
#include "Main.h"
#include "InputHandler.h"
#include "Utils.h"

XPLMWindowID FMCWindow = NULL;
XPLMHotKeyID FMCToggleHotKey = NULL;

bool FMCDisplayWindow = false;
bool FMCKeyboardInput = false;

XPLMTextureID Texture[MAX_TEXTURES];

Pages* pages;
Flight* flight;

/* Required code for compiling on Windows, thanks to Pikitanga / Duncan */
#if IBM
BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }

  return TRUE;
}
#endif

PLUGIN_API int XPluginStart(char * outName, char * outSig, char * outDesc) {
  int x = 0; int y = 720;
  int width = 360; int height = 570;

  Page * page;

  XPLMEnableFeature("XPLM_USE_NATIVE_PATHS",1);

  strcpy(outName, "BasicFMC");
  strcpy(outSig, "BasicFMC");
  strcpy(outDesc, "BasicFMC by Bo Simonsen.");

  FMCWindow = XPLMCreateWindow(x, y, x + width, y - height, 1,
                               FMCWindowCallback, FMCKeyCallback, 
                               FMCMouseClickCallback, NULL);

  FMCToggleHotKey = XPLMRegisterHotKey(XPLM_VK_F8, xplm_DownFlag,
                                       "F8", FMCToggleHotKeyHandler, NULL);

  XPLMRegisterFlightLoopCallback(FMCLoopCallback, 1.0, NULL);
  
  int result = LoadGLTexture(std::string("fmc.bmp"), 0);

  if(!result) {
    XPLMDebugString("Could not load FMC texture");
  }

  flight = new Flight();
  pages = new Pages();
    
  page = new InitPage(flight); pages->RegisterPage("init", page);
  page = new LegsPage(flight); pages->RegisterPage("legs", page);
  page = new AirportPage(flight); pages->RegisterPage("airport", page);
  page = new DepPage(flight); pages->RegisterPage("airport_dep", page);
  page = new ArrPage(flight); pages->RegisterPage("airport_arr", page);
  page = new ProgressPage(flight); pages->RegisterPage("progress", page);
  
  pages->SwitchPage("init");
  
  return 1;
}

PLUGIN_API void	XPluginStop(void) {
  XPLMUnregisterHotKey(FMCToggleHotKey);
  XPLMDestroyWindow(FMCWindow);
  XPLMUnregisterFlightLoopCallback(FMCLoopCallback, NULL);

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
    XPLMGetWindowGeometry(FMCWindow, &PanelWindowLeft,
                          &PanelWindowTop, &PanelWindowRight,
                          &PanelWindowBottom);

    PanelLeft = PanelWindowLeft; 
    PanelRight = PanelWindowRight; 
    PanelBottom = PanelWindowBottom; 
    PanelTop = PanelWindowTop;
    
    XPLMSetGraphicsState(0, 1, 0, 0, 0, 0, 0);
    
    XPLMBindTexture2d(Texture[FMC_TEXTURE], 0);
    
    glPushMatrix();
    glBegin(GL_QUADS);

    glTexCoord2f(1, 0.0f); glVertex2f(PanelRight, PanelBottom);
    glTexCoord2f(0, 0.0f); glVertex2f(PanelLeft, PanelBottom);
    glTexCoord2f(0, 1.0f); glVertex2f(PanelLeft, PanelTop);
    glTexCoord2f(1, 1.0f); glVertex2f(PanelRight, PanelTop);

    glEnd();
    glPopMatrix();

    XPLMSetGraphicsState(0, 0, 0, 0, 0, 0, 0);
    
    glFlush();

    Page * page = pages->CurrentPage();
    page->SetCoordinates(&PanelLeft, &PanelTop);
    page->Update();
  }
}

void FMCKeyCallback(XPLMWindowID inWindowID, char inKey, XPLMKeyFlags inFlags, char inVirtualKey, void * inRefcon, int losingFocus) {

  if(!FMCKeyboardInput) {
    return;
  }
  
  Page* page = pages->CurrentPage();

  // accept key event if key is pressed down or held down (from X-Ivap)
  if(((inFlags & xplm_DownFlag) != 0) ||
     ((inFlags & xplm_DownFlag) == 0 &&
      (inFlags & xplm_UpFlag) == 0)) {

    switch(inKey) {
    case 8:
      page->HandleDelete();
      break;
    default:
      page->HandleInput(inKey);
      break;
    }
  }
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
      Left = (x - dX); Right = Left + Weight;
      Top = (y - dY); Bottom = Top + Height;
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

float FMCLoopCallback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void *inRefcon) {
  int destination = XPLMGetDestinationFMSEntry();

  XPLMDataRef lat_ref = XPLMFindDataRef("sim/flightmodel/position/latitude");
  XPLMDataRef lon_ref = XPLMFindDataRef("sim/flightmodel/position/longitude");

  float lat = XPLMGetDataf(lat_ref);
  float lon = XPLMGetDataf(lon_ref);

  XPLMSetFMSEntryLatLon(0, lat, lon, 0);
  
  if(destination > 1) {
    while(destination > 1 && flight->flightplan.size() > 0) {
      flight->flightplan.erase(flight->flightplan.begin());
      destination--;
    }

    flight->SyncToXPFMC();
  }

  flight->CalcDistances();
  
  return 0.5;
}




