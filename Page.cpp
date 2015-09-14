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

#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "XPLMDefs.h"
#include "XPLMDisplay.h"
#include "XPLMDataAccess.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"

#include "Page.h"

extern bool FMCKeyboardInput;

Page::Page(Flight* flight) {
  this->flight = flight;
}

void Page::SetCoordinates(float* left, float* top) {
  this->left = left;
  this->top = top;
}

void Page::DrawString(float x, float y, std::string& str) {
  char* c_string = (char*) str.c_str();
  XPLMDrawString((float*) color_green, x, y, c_string, NULL, xplmFont_Basic);
}

std::string Page::FormatString(std::string left, std::string right) {
  std::stringstream ss;

  std::transform(left.begin(), left.end(), left.begin(), ::toupper);
  std::transform(right.begin(), right.end(), right.begin(), ::toupper);
    
  ss << std::setw(17) << std::left << left;
  ss << std::setw(17) << std::right << right;

  return ss.str();
}

void Page::Draw() {
  float x;
  float y;
    
  x = *this->left;
  y = *this->top;

  this->DrawString(x+70, y-40, this->heading);
  
  this->DrawString(x+70, y-65, this->line1_h);
  this->DrawString(x+70, y-80, this->line1);
  
  this->DrawString(x+70, y-95, this->line2_h);
  this->DrawString(x+70, y-110, this->line2);
  
  this->DrawString(x+70, y-125, this->line3_h);
  this->DrawString(x+70, y-140, this->line3);
  
  this->DrawString(x+70, y-155, this->line4_h);
  this->DrawString(x+70, y-170, this->line4);
  
  this->DrawString(x+70, y-185, this->line5_h);
  this->DrawString(x+70, y-200, this->line5);
  
  this->DrawString(x+70, y-215, this->line6_h);
  this->DrawString(x+70, y-230, this->line6);

  std::string end_line;
  if(this->error.length() > 0) {
    end_line = this->error;
  }
  else {
    end_line = this->FormatString(this->input, this->GetStatus());
  }
  
  this->DrawString(x+70, y-250, end_line);
}

void Page::HandleInput(char input_char) {
  if(this->error.length() == 0) {
    this->input += toupper(input_char);
  }
}

bool Page::HandleDelete() {
  if(this->error.length() > 0) {
    this->error.clear();
    return true;
  }
  else if(this->input.length() > 0) {
    this->input.erase(this->input.end() - 1);
    return true;
  }

  return false;
}

std::string Page::GetStatus() {
  std::string status;

  if(this->error.length() > 0) {
    status = "ERR";
  }
  if(FMCKeyboardInput) {
    if(status.length() > 0) {
      status += ",KEY";
    }
    else {
      status = "KEY";
    }
  }
  return status;
}
