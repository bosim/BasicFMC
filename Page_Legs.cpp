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

#include "Page_Legs.h"

#include <cmath>
#include <sstream>

LegsPage::LegsPage(Flight* flight) : Page(flight) {
  this->offset = 0;
}

void LegsPage::PrintLine(unsigned int offset, std::string* line,
                         std::vector<NavAidInfo>* flightplan) {
  if(offset < (*flightplan).size()) {
    (*line) = this->FormatString((*flightplan)[offset].id,
                                 "--- / ----");
  } else {
    (*line).clear();
  }
  
}

void LegsPage::Update() {
  std::vector<NavAidInfo>* flightplan;
  
  flightplan = &this->flight->flightplan;

  unsigned int current_page = ceil(float(this->offset) / 6) + 1;
  unsigned int total_pages = ceil(float((*flightplan).size()) / 6) + 1;

  std::stringstream ss;

  ss << current_page << "/" << total_pages;
  
  this->heading = this->FormatString(std::string("Flightplan"),
                                     ss.str());
  
  this->PrintLine(this->offset, &this->line1, flightplan);
  this->PrintLine(this->offset + 1, &this->line2, flightplan);
  this->PrintLine(this->offset + 2, &this->line3, flightplan);
  this->PrintLine(this->offset + 3, &this->line4, flightplan);
  this->PrintLine(this->offset + 4, &this->line5, flightplan);
  this->PrintLine(this->offset + 5, &this->line6, flightplan);
  
  this->Draw();
}

void LegsPage::HandleSK(int key) {
  int index;
  std::vector<NavAidInfo>* flightplan;

  flightplan = &this->flight->flightplan;

  switch(key) {
  case BUTTON_UP:
    if(this->offset > 0) {
      offset--;
    }
    return;
  case BUTTON_DOWN:
    if(this->offset + 1 < (*flightplan).size()) {
      offset++;
    }
    return;
  }
  

  std::vector<NavAidInfo> result;
  Navigation::FindNavAid(this->input, result);

  if(result.size() == 0) {
    this->error = "Navaid could not be found";
    return;
  }
  
  switch(key) {
  case LSK1: index = 0; break;
  case LSK2: index = 1; break;
  case LSK3: index = 2; break;
  case LSK4: index = 3; break;
  case LSK5: index = 4; break;
  case LSK6: index = 5; break;
  default:
    return;
  }

  unsigned int insert_index = this->offset + index + 1;
  if(insert_index <= (*flightplan).size()) {
    (*flightplan).insert((*flightplan).begin() + insert_index,
                         result[0]);
  } else {
    (*flightplan).push_back(result[0]);
  }

  this->input.clear();
  
  this->flight->SyncToXPFMC();
}
