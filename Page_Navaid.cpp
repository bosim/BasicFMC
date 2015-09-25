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

#include "Pages.h"
#include "Page_Navaid.h"

#include <cmath>
#include <sstream>

extern Pages * pages;

NavaidPage::NavaidPage(Flight* flight) : Page(flight), offset(0) {

}

void NavaidPage::PrintLine(unsigned int offset, std::string* line,
                         std::vector<NavAidInfo>* flightplan) {
  std::stringstream ss;
  std::string right_column;

  if(offset < (*flightplan).size()) {
    ss << (*flightplan)[offset].lat << " , " << (*flightplan)[offset].lon;

    right_column = ss.str();
    (*line) = this->FormatString((*flightplan)[offset].id,
                                 right_column);
  }
}

void NavaidPage::Update() {
  std::vector<NavAidInfo>* navaids;
  
  navaids = &this->flight->temp_navaids;

  unsigned int current_page = ceil(float(this->offset) / 5) + 1;
  unsigned int total_pages = ceil(float((*navaids).size()) / 5);

  this->Clear();

  this->heading = this->GenerateHeading("Select navaids", current_page, total_pages);
  
  this->PrintLine(this->offset, &this->line1, navaids);
  this->PrintLine(this->offset + 1, &this->line2, navaids);
  this->PrintLine(this->offset + 2, &this->line3, navaids);
  this->PrintLine(this->offset + 3, &this->line4, navaids);
  this->PrintLine(this->offset + 4, &this->line5, navaids);
  
  this->Draw();

}

void NavaidPage::HandleSK(int key) {
  int index;

  switch(key) {
  case BUTTON_UP:
    if(static_cast<int>(this->offset) - 5 > 0) {
      this->offset = this->offset - 5;
    }
    else {
      this->offset = 0;
    }
    return;
  case BUTTON_DOWN:
    if(this->offset + 5 < this->flight->temp_navaids.size()) {
      this->offset = this->offset + 5;
    }
    return;
  case LSK1: index = 0; break;
  case LSK2: index = 1; break;
  case LSK3: index = 2; break;
  case LSK4: index = 3; break;
  case LSK5: index = 4; break;
  //case LSK6: index = 5; break;
  default:
    return;
  }

  unsigned int navaid_index = this->offset + index;

  std::vector<NavAidInfo>* flightplan = &this->flight->flightplan;

  if(this->flight->temp_navaid_insert + 1 <= (*flightplan).size()) {
    (*flightplan).insert((*flightplan).begin() + this->flight->temp_navaid_insert + 1, this->flight->temp_navaids[navaid_index]);
  } else {
    (*flightplan).push_back(this->flight->temp_navaids[navaid_index]);
  }

  this->input.clear();
  this->flight->SyncToXPFMC();

  /* Let us change mode to LEGS when the waypoint was successfully added */
  pages->SwitchPage(this->flight->temp_navaid_came_from);
}

bool NavaidPage::OnSwitch() {
  this->offset = 0;
  return true;
}
