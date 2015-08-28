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

LegsPage::LegsPage(Flight* flight) : Page(flight) {
  this->heading = this->FormatString(std::string("Flightplan"),
                                     std::string("1/1"));
  this->offset = 0;
}

void LegsPage::Update() {
  std::vector<NavAidInfo>* flightplan;

  flightplan = &this->flight->flightplan;

  if(this->offset < (*flightplan).size()) {
    this->line1 = this->FormatString((*flightplan)[offset].id,
                                     "--- / ----");
  }
  if(this->offset + 1 < (*flightplan).size()) {
    this->line2 = this->FormatString((*flightplan)[offset+1].id,
                                     "--- / ----");
  }
  if(this->offset + 2 < (*flightplan).size()) {
    this->line3 = this->FormatString((*flightplan)[offset+2].id,
                                     "--- / ----");
  }
  if(this->offset + 3 < (*flightplan).size()) {
    this->line4 = this->FormatString((*flightplan)[offset+3].id,
                                     "--- / ----");
  }
  if(this->offset + 4 < (*flightplan).size()) {
    this->line5 = this->FormatString((*flightplan)[offset+4].id,
                                     "--- / ----");
  }
  if(this->offset + 5 < (*flightplan).size()) {
    this->line6 = this->FormatString((*flightplan)[offset+5].id,
                                     "--- / ----");
  }
  
  this->Draw();
}

void LegsPage::HandleSK(int key) {
  int index;
  std::vector<NavAidInfo>* flightplan;

  flightplan = &this->flight->flightplan;

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

  this->flight->SyncToXPFMC();
}
