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

#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "Flight.h"

float	color_green[] = { 0.0, 1.0, 0.0 };

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
  Page(Flight* flight) {
    this->flight = flight;
  }

  void SetCoordinates(float* left, float* top) {
    this->left = left;
    this->top = top;
  };

  void DrawString(float x, float y, std::string& str) {
    char* c_string = (char*) str.c_str();
    XPLMDrawString(color_green, x, y, c_string, NULL, xplmFont_Basic);
  }

  std::string FormatString(std::string left, std::string right) {
    std::stringstream ss;

    std::transform(left.begin(), left.end(), left.begin(), ::toupper);
    std::transform(right.begin(), right.end(), right.begin(), ::toupper);
    
    ss << std::setw(17) << std::left << left;
    ss << std::setw(17) << std::right << right;

    return ss.str();
  }
  
  void Draw() {
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

    this->DrawString(x+70, y-250, this->input);
  }

  virtual void Update() {
    throw std::logic_error("Not implemented");
  }

  void HandleInput(char input_char) {
    this->input += input_char;
  }

  virtual void HandleSK(const int key) {
    throw std::logic_error("Not implemented");
  }
  
  void HandleDelete() {
    if(this->input.length() > 0) {
      this->input.pop_back();
    }
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
};

#endif
