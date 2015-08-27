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

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

extern Pages* pages;
extern Flight* flight;

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
  else if(CoordInRect(dX, dY, 10, -99, 33, -115)) {
    page->HandleSK(LSK2);
  }
  else if(CoordInRect(dX, dY, 10, -127, 33, -143)) {
    page->HandleSK(LSK3);
  }
  else if(CoordInRect(dX, dY, 10, -156, 33, -173)) {
    page->HandleSK(LSK4);
  }
  else if(CoordInRect(dX, dY, 10, -183, 33, -200)) {
    page->HandleSK(LSK5);
  }
  else if(CoordInRect(dX, dY, 10, -211, 33, -229)) {
    page->HandleSK(LSK6);
  }
  else if(CoordInRect(dX, dY, 325, -71, 351, -87)) {
    page->HandleSK(RSK1);
  }
  else if(CoordInRect(dX, dY, 325, -99, 351, -115)) {
    page->HandleSK(RSK2);
  }
  else if(CoordInRect(dX, dY, 325, -127, 351, -143)) {
    page->HandleSK(RSK3);
  }
  else if(CoordInRect(dX, dY, 325, -156, 351, -173)) {
    page->HandleSK(RSK4);
  }
  else if(CoordInRect(dX, dY, 325, -183, 351, -200)) {
    page->HandleSK(RSK5);
  }
  else if(CoordInRect(dX, dY, 325, -211, 351, -229)) {
    page->HandleSK(RSK6);
  }

}

#endif
