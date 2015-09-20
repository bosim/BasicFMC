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

#include "Page_Route.h"
#include "Utils.h"

RoutePage::RoutePage(Flight* flight) : Page(flight), offset(0) {
  this->airway_reader = AirwayReader();

  /* Logic for using custom data for airways */
  bool result = this->airway_reader.setFilename(GetAirwayFilename(true));

  if(!result) {
    XPLMDebugString("BasicFMC: Custom data not found using default airways\n");
    this->airway_reader.setFilename(GetAirwayFilename(false));
  }
  else {
    XPLMDebugString("BasicFMC: Custom data found using it for airways\n");
  }

}

void RoutePage::PrintLine(unsigned int offset, std::string* line) {
  if(offset < this->rtes.size()) {
    (*line) = this->FormatString(this->rtes[offset].airway,
                                 this->rtes[offset].dest);
  }
  else {
    (*line).clear();
  }
}

void RoutePage::Update() {

  this->PrintLine(this->offset, &this->line1);
  this->PrintLine(this->offset + 1, &this->line2);
  this->PrintLine(this->offset + 2, &this->line3);
  this->PrintLine(this->offset + 3, &this->line4);
  this->PrintLine(this->offset + 4, &this->line5);

  this->Draw();
}

void RoutePage::HandleSK(int key) {
  int airway_index = -1;
  int dest_index = -1;
  switch(key) {
  case LSK1: airway_index = 0; break;
  case LSK2: airway_index = 1; break;
  case LSK3: airway_index = 2; break;
  case LSK4: airway_index = 3; break;
  case LSK5: airway_index = 4; break;
  case RSK1: dest_index = 0; break;
  case RSK2: dest_index = 1; break;
  case RSK3: dest_index = 2; break;
  case RSK4: dest_index = 3; break;
  case RSK5: dest_index = 4; break;
  default: return;
  }

  if(airway_index >= 0) {
    unsigned int operation_index = this->offset + airway_index;
    if(operation_index < this->rtes.size()) {
      /* Todo */
    }
    else {
      ItemRTE item;
      item.airway = this->input;
      this->rtes.push_back(item);
    }
  }
  else if(dest_index >= 0) {
    unsigned int operation_index = this->offset + dest_index;
    if(operation_index < this->rtes.size()) {
      this->rtes[operation_index].dest = this->input;
      
      std::string source = this->rtes[operation_index-1].dest;
      std::string airway = this->rtes[operation_index].airway;
      std::string dest = this->rtes[operation_index].dest;
      
      std::vector<NavAidInfo> waypoints;
      this->airway_reader.FindSegment(airway, source, dest, waypoints);
      
      if(waypoints.size() == 0) {
        this->error = "Airway not found";
        return;
      }

      unsigned int insert_index = this->rtes[operation_index-1].end_index;

      std::vector<NavAidInfo>* flightplan = &this->flight->flightplan;
      
      if(insert_index + 1 <= (*flightplan).size()) {
        for(size_t i=waypoints.size()-1; i > 0; i--) {
          (*flightplan).insert((*flightplan).begin() + insert_index + 1,
                               waypoints[i]);
        }
      } else {
        for(size_t i=1; i < waypoints.size(); i++) {
          (*flightplan).push_back(waypoints[i]);
        }
      }
      this->input.clear();
      this->GenerateRTEs();
    }
  }
}

bool RoutePage::OnSwitch() {
  this->GenerateRTEs();
  return true;
}

void RoutePage::GenerateRTEs() {
  this->rtes.clear();
  std::vector<NavAidInfo>* flightplan = &this->flight->flightplan;
  
  for(size_t i=0; i < flightplan->size();) {
    if((*flightplan)[i].fmc_sid) {
      ItemRTE item;
      
      item.airway = "";
      item.dest = (*flightplan)[i].id;
      item.end_index = i;

      for(i++; i < flightplan->size(); i++) {
        if((*flightplan)[i].fmc_sid) {
          item.dest = (*flightplan)[i].id;
          item.end_index = i;
        }
        else {
          break;
        }
      }
      this->rtes.push_back(item);

    }
    else if((*flightplan)[i].fmc_star) {
      ItemRTE item;

      item.airway = "";
      item.dest = (*flightplan)[i].id;
      item.end_index = i;

      for(i++; i < flightplan->size(); i++) {
        if((*flightplan)[i].fmc_star) {
          item.dest = (*flightplan)[i].id;
          item.end_index = i;
        }
        else {
          break;
        }
      }
      this->rtes.push_back(item);
    }    
    else if((*flightplan)[i].fmc_airway.size() > 0) {
      ItemRTE item;
      int airway_start = i;

      item.airway = (*flightplan)[airway_start].fmc_airway;
      item.dest = (*flightplan)[i].id;
      item.end_index = i;

      for(i++; i < flightplan->size(); i++) {
        if((*flightplan)[airway_start].fmc_airway == (*flightplan)[i].fmc_airway) {
          item.dest = (*flightplan)[i].id;
          item.end_index = i;
        }
        else {
          break;
        }
      }

      this->rtes.push_back(item);
    }
    else if((*flightplan)[i].fmc_airway.size() == 0) {
      ItemRTE item;
      item.airway = "";
      item.dest = (*flightplan)[i].id;
      item.end_index = i;
      this->rtes.push_back(item);
      i++;
    }
  }
}
