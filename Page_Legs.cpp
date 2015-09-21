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
  this->navaid_offset = 0;
  this->delete_mode = false;
  this->mode = MODE_LEGS;
}

void LegsPage::PrintLine(unsigned int offset, std::string* line,
                         std::vector<NavAidInfo>* flightplan,
                         bool coordinates=false) {

  std::stringstream ss;
  std::string right_column;

  if(offset < (*flightplan).size()) {
    if(coordinates) {
      ss << (*flightplan)[offset].lat << " , " << (*flightplan)[offset].lon;
    }
    else {
      ss.str("");

      if((*flightplan)[offset].fmc_forced_speed) {
        ss << (*flightplan)[offset].fmc_forced_speed;
      }
      else {
        ss << "---";
      }
      
      ss << " / ";
      
      if((*flightplan)[offset].fmc_forced_altitude) {
        ss << (*flightplan)[offset].fmc_forced_altitude;
      }
      else {
        ss << "-----";
      }
    }
    right_column = ss.str();
    (*line) = this->FormatString((*flightplan)[offset].id,
                                 right_column);
  } 
  
}

void LegsPage::LegsUpdate() {
  std::vector<NavAidInfo>* flightplan;
  
  flightplan = &this->flight->flightplan;

  unsigned int current_page = ceil(float(this->offset) / 5) + 1;
  unsigned int total_pages = floor(float((*flightplan).size()) / 5) + 1;

  this->Clear();

  this->heading = this->GenerateHeading("Legs", current_page, total_pages);
  
  this->PrintLine(this->offset, &this->line1, flightplan);
  this->PrintLine(this->offset + 1, &this->line2, flightplan);
  this->PrintLine(this->offset + 2, &this->line3, flightplan);
  this->PrintLine(this->offset + 3, &this->line4, flightplan);
  this->PrintLine(this->offset + 4, &this->line5, flightplan);
  
  this->Draw();
}

void LegsPage::NavaidUpdate() {
  std::vector<NavAidInfo>* navaids;
  
  navaids = &this->navaids;

  unsigned int current_page = ceil(float(this->navaid_offset) / 5) + 1;
  unsigned int total_pages = floor(float((*navaids).size()) / 5) + 1;

  this->Clear();

  this->heading = this->GenerateHeading("Select navaids", current_page, total_pages);
  
  this->PrintLine(this->navaid_offset, &this->line1, navaids, true);
  this->PrintLine(this->navaid_offset + 1, &this->line2, navaids, true);
  this->PrintLine(this->navaid_offset + 2, &this->line3, navaids, true);
  this->PrintLine(this->navaid_offset + 3, &this->line4, navaids, true);
  this->PrintLine(this->navaid_offset + 4, &this->line5, navaids, true);
  
  this->Draw();

}

void LegsPage::Update() {
  switch(this->mode) {
  case MODE_LEGS:
    LegsUpdate();
    break;
  case MODE_NAVAID:
    NavaidUpdate();
    break;
  default:
    throw std::logic_error("Mode has wrong value!");
  }
}

void LegsPage::LegsHandleSK(int key) {
  int index;
  std::vector<NavAidInfo>* flightplan;

  flightplan = &this->flight->flightplan;
  
  switch(key) {
  case BUTTON_UP:
    if(static_cast<int>(this->offset) - 5 > 0) {
      this->offset = this->offset - 5;
    } else {
      this->offset = 0;
    }
    return;
  case BUTTON_DOWN:
    if(this->offset + 5 <= (*flightplan).size()) {
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

  unsigned int operation_index = this->offset + index;
  
  if(!this->delete_mode) {
    /* We can press LSK and if no input the current navaid is copied to input */
    if(this->input.size() == 0 && operation_index < this->flight->flightplan.size()) {
      this->input = this->flight->flightplan[operation_index].id;
      return;
    }
    else if(this->input.size() == 0) {
      return;
    }

    int found_index = -1;

    for(size_t i = 0; i < this->flight->flightplan.size(); i++) {
      if(this->flight->flightplan[i].id == this->input) {
        if(found_index > 0) {
          this->error = "Duplicate waypoint aborting";
          return;
        }

        found_index = i;
      }
    }

    if(found_index > 0) {
      this->flight->flightplan.erase(this->flight->flightplan.begin() + operation_index,
                                     this->flight->flightplan.begin() + found_index);
      this->input.clear();
      return;
    }

    /* Clear navaids storage to ensure consistency */
    this->navaids.clear();

    Navigation::FindNavAid(this->input, this->navaids);

    if(this->navaids.size() == 0) {
      this->error = "Navaid could not be found";
      return;
    }

    if(this->navaids.size() > 1) {
      this->operation_index = this->offset + index;
      this->navaid_offset = 0;
      this->mode = MODE_NAVAID;
      return;
    }

    if(operation_index + 1 <= (*flightplan).size()) {
      (*flightplan).insert((*flightplan).begin() + operation_index + 1,
                           this->navaids[0]);
    } else {
      (*flightplan).push_back(this->navaids[0]);
    }

    this->input.clear();
  }
  else {
    if(operation_index < (*flightplan).size()) {
      (*flightplan).erase((*flightplan).begin() + operation_index);
    }

    this->delete_mode = false;
  }

  this->flight->SyncToXPFMC();
}

void LegsPage::NavaidHandleSK(int key) {
  int index;

  switch(key) {
  case BUTTON_UP:
    if(static_cast<int>(this->navaid_offset) - 5 > 0) {
      this->navaid_offset = this->navaid_offset - 5;
    }
    else {
      this->navaid_offset = 0;
    }
    return;
  case BUTTON_DOWN:
    if(this->navaid_offset + 5 < this->navaids.size()) {
      this->navaid_offset = this->navaid_offset + 5;
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

  unsigned int navaid_index = this->navaid_offset + index;

  std::vector<NavAidInfo>* flightplan = &this->flight->flightplan;

  if(this->operation_index + 1 <= (*flightplan).size()) {
    (*flightplan).insert((*flightplan).begin() + this->operation_index + 1,
                         navaids[navaid_index]);
  } else {
    (*flightplan).push_back(navaids[navaid_index]);
  }

  this->input.clear();
  this->flight->SyncToXPFMC();

  /* Let us change mode to LEGS when the waypoint was successfully added */
  this->mode = MODE_LEGS;
}

void LegsPage::HandleSK(int key) {
  switch(this->mode) {
  case MODE_LEGS:
    LegsHandleSK(key);
    break;
  case MODE_NAVAID:
    NavaidHandleSK(key);
    break;
  }
}

bool LegsPage::NavaidHandleDelete() {
  /* Delete will cancel */
  this->mode = MODE_LEGS;
  return true;
}

bool LegsPage::LegsHandleDelete() {
  bool action = Page::HandleDelete();
  if(!action) {
    this->delete_mode = !this->delete_mode;
  }
  return true;
}

bool LegsPage::HandleDelete() {
  switch(this->mode) {
  case MODE_LEGS:
    return LegsHandleDelete();
    break;
  case MODE_NAVAID:
    return NavaidHandleDelete();
    break;
  }

  return true;
}

std::string LegsPage::GetStatus() {
  std::string status = Page::GetStatus();
  if(this->delete_mode) {
    if(status.length() > 0) {
      status += ", DEL";
    }
    else {
      status = "DEL";
    }
  }
  return status;
}
