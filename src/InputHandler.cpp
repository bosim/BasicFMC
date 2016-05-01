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

#include "XPLMDefs.h"
#include "XPLMDisplay.h"
#include "XPLMDataAccess.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"

#include "Utils.h"
#include "Pages.h"
#include "Page.h"
#include "InputHandler.h"

extern Pages* pages;
extern Flight* flight;
extern XPLMWindowID FMCWindow;
extern bool FMCKeyboardInput;

void ToggleKeyboard() {
  
  FMCKeyboardInput = !FMCKeyboardInput;

  if(FMCKeyboardInput) {
    XPLMBringWindowToFront(FMCWindow);
    XPLMTakeKeyboardFocus(FMCWindow);
  }
  else {
    XPLMTakeKeyboardFocus(0);    
  }
}

void InputHandler(int &dX, int &dY) {
  Page* page = pages->CurrentPage();
  
  if(CoordInRect(dX, dY, 152, -337, 174, -359)) {
    page->HandleInput('A');
  }
  else if(CoordInRect(dX, dY, 187, -337, 209, -359)) {
    page->HandleInput('B');
  }
  else if(CoordInRect(dX, dY, 221, -337, 245, -359)) {
    page->HandleInput('C');
  }
  else if(CoordInRect(dX, dY, 257, -337, 280, -359)) {
    page->HandleInput('D');
  }
  else if(CoordInRect(dX, dY, 292, -337, 317, -359)) {
    page->HandleInput('E');
  }
  else if(CoordInRect(dX, dY, 152, -372, 174, -392)) {
    page->HandleInput('F');
  }
  else if(CoordInRect(dX, dY, 187, -372, 209, -392)) {
    page->HandleInput('G');
  }
  else if(CoordInRect(dX, dY, 221, -372, 245, -392)) {
    page->HandleInput('H');
  }
  else if(CoordInRect(dX, dY, 257, -372, 280, -392)) {
    page->HandleInput('I');
  }
  else if(CoordInRect(dX, dY, 292, -372, 317, -392)) {
    page->HandleInput('J');
  }
  else if(CoordInRect(dX, dY, 152, -407, 174, -428)) {
    page->HandleInput('K');
  }
  else if(CoordInRect(dX, dY, 187, -407, 209, -428)) {
    page->HandleInput('L');
  }
  else if(CoordInRect(dX, dY, 221, -407, 245, -428)) {
    page->HandleInput('M');
  }
  else if(CoordInRect(dX, dY, 257, -407, 280, -428)) {
    page->HandleInput('N');
  }
  else if(CoordInRect(dX, dY, 292, -407, 317, -428)) {
    page->HandleInput('O');
  }
  else if(CoordInRect(dX, dY, 152, -441, 174, -463)) {
    page->HandleInput('P');
  }
  else if(CoordInRect(dX, dY, 187, -441, 209, -463)) {
    page->HandleInput('Q');
  }
  else if(CoordInRect(dX, dY, 221, -441, 245, -463)) {
    page->HandleInput('R');
  }
  else if(CoordInRect(dX, dY, 257, -441, 280, -463)) {
    page->HandleInput('S');
  }
  else if(CoordInRect(dX, dY, 292, -441, 317, -463)) {
    page->HandleInput('T');
  }
  else if(CoordInRect(dX, dY, 152, -476, 174, -498)) {
    page->HandleInput('U');
  }
  else if(CoordInRect(dX, dY, 187, -476, 209, -498)) {
    page->HandleInput('V');
  }
  else if(CoordInRect(dX, dY, 221, -476, 245, -498)) {
    page->HandleInput('W');
  }
  else if(CoordInRect(dX, dY, 257, -476, 280, -498)) {
    page->HandleInput('X');
  }
  else if(CoordInRect(dX, dY, 292, -476, 317, -498)) {
    page->HandleInput('Y');
  }
  else if(CoordInRect(dX, dY, 152, -511, 174, -532)) {
    page->HandleInput('Z');
  }
  else if(CoordInRect(dX, dY, 187, -511, 209, -532)) {
    page->HandleInput(' ');
  }
  else if(CoordInRect(dX, dY, 221, -511, 245, -532)) {
    page->HandleDelete();
  }
  else if(CoordInRect(dX, dY, 257, -511, 280, -532)) {
    page->HandleInput('/');
  }
  else if(CoordInRect(dX, dY, 292, -511, 317, -532)) {
    // TODO handle clear
  }
  else if(CoordInRect(dX, dY, 43, -403, 71, -428)) {
    page->HandleInput('1');
  }
  else if(CoordInRect(dX, dY, 79, -403, 107, -428)) {
    page->HandleInput('2');
  }
  else if(CoordInRect(dX, dY, 112, -403, 141, -428)) {
    page->HandleInput('3');
  }
  else if(CoordInRect(dX, dY, 43, -439, 71, -463)) {
    page->HandleInput('4');
  }
  else if(CoordInRect(dX, dY, 79, -439, 107, -463)) {
    page->HandleInput('5');
  }
  else if(CoordInRect(dX, dY, 112, -439, 141, -463)) {
    page->HandleInput('6');
  }
  else if(CoordInRect(dX, dY, 43, -474, 71, -499)) {
    page->HandleInput('7');
  }
  else if(CoordInRect(dX, dY, 79, -474, 107, -499)) {
    page->HandleInput('8');
  }
  else if(CoordInRect(dX, dY, 112, -474, 141, -499)) {
    page->HandleInput('9');
  }
  else if(CoordInRect(dX, dY, 43, -509, 71, -533)) {
    page->HandleInput('.');
  }
  else if(CoordInRect(dX, dY, 79, -509, 107, -533)) {
    page->HandleInput('0');
  }
  else if(CoordInRect(dX, dY, 112, -509, 141, -533)) {
    page->HandleInput('+');
  }
  else if(CoordInRect(dX, dY, 5, -64, 25, -77)) {
    page->HandleSK(LSK1);
  }
  else if(CoordInRect(dX, dY, 5, -92, 25, -107)) {
    page->HandleSK(LSK2);
  }
  else if(CoordInRect(dX, dY, 5, -120, 25, -136)) {
    page->HandleSK(LSK3);
  }
  else if(CoordInRect(dX, dY, 5, -149, 25, -165)) {
    page->HandleSK(LSK4);
  }
  else if(CoordInRect(dX, dY, 5, -178, 25, -193)) {
    page->HandleSK(LSK5);
  }
  else if(CoordInRect(dX, dY, 5, -205, 25, -223)) {
    page->HandleSK(LSK6);
  }
  else if(CoordInRect(dX, dY, 335, -64, 356, -77)) {
    page->HandleSK(RSK1);
  }
  else if(CoordInRect(dX, dY, 335, -92, 356, -107)) {
    page->HandleSK(RSK2);
  }
  else if(CoordInRect(dX, dY, 335, -120, 356, -136)) {
    page->HandleSK(RSK3);
  }
  else if(CoordInRect(dX, dY, 335, -149, 356, -165)) {
    page->HandleSK(RSK4);
  }
  else if(CoordInRect(dX, dY, 335, -178, 356, -193)) {
    page->HandleSK(RSK5);
  }
  else if(CoordInRect(dX, dY, 335, -205, 356, -223)) {
    page->HandleSK(RSK6);
  }
  else if(CoordInRect(dX, dY, 47, -263, 81, -286)) {
    pages->SwitchPage("init");
  }
  else if(CoordInRect(dX, dY, 92, -299, 126, -321)) {
    pages->SwitchPage("legs");
  }
  else if(CoordInRect(dX, dY, 139, -264, 171, -286)) {
    pages->SwitchPage("airport");
  }
  else if(CoordInRect(dX, dY, 229, -297, 263, -321)) {
    pages->SwitchPage("progress");
  }
  else if(CoordInRect(dX, dY, 92, -264, 126, -286)) {
    pages->SwitchPage("route");
  }
  else if(CoordInRect(dX, dY, 47, -369, 80, -390)) {
    page->HandleSK(BUTTON_UP);
  }
  else if(CoordInRect(dX, dY, 93, -370, 126, -391)) {
    page->HandleSK(BUTTON_DOWN);
  }
  else if(CoordInRect(dX, dY, 0, -18, 24, -44)) {
    ToggleKeyboard();
  }
}
