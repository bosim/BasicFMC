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

#ifndef PAGE_H
#define PAGE_H

#include <stdexcept>

#include "Flight.h"

const float color_green[] = { 0.0, 1.0, 0.0 };

const int LSK1 = 1;
const int LSK2 = 2;
const int LSK3 = 3;
const int LSK4 = 4;
const int LSK5 = 5;
const int LSK6 = 6;
const int RSK1 = 7;
const int RSK2 = 8;
const int RSK3 = 9;
const int RSK4 = 10;
const int RSK5 = 11;
const int RSK6 = 12;

class Page {

 public:
  Page(Flight* flight);
  
  void SetCoordinates(float* left, float* top);
  void DrawString(float x, float y, std::string& str);
  std::string FormatString(std::string left, std::string right);  
  void Draw();
  void HandleInput(char input_char);
  void HandleDelete();
  
  virtual void Update() {
    throw std::logic_error("Not implemented");
  }

  virtual void HandleSK(const int key) {
    throw std::logic_error("Not implemented");
  }
  
 protected:
  Flight* flight;

  float* left;
  float* top;

  std::string heading;
  
  std::string line1_h;
  std::string line1;
  std::string line2_h;
  std::string line2;
  std::string line3_h;
  std::string line3;
  std::string line4_h;
  std::string line4;
  std::string line5_h;
  std::string line5;
  std::string line6_h;
  std::string line6;

  std::string input;
  std::string error;
};

#endif
