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

#include "Page_Route.h"
#include "Utils.h"

#include <cmath>

extern Pages * pages;

RoutePage::RoutePage(Flight* flight) : Page(flight), offset(0), delete_mode(false), airway_reader(AirwayReader(GetAirwayFilename())) {
}

void RoutePage::PrintLine(unsigned int offset, std::string* line) {
  if(offset < this->routes.size()) {

    std::string left_column, right_column;

    if(this->routes[offset].sid) {
      left_column = "SID";
    }
    else if(this->routes[offset].star) {
      left_column = "STAR";
    }
    else if(this->routes[offset].airway.size() > 0) {
      left_column = this->routes[offset].airway;
    }
    else {
      left_column = "DCT";
    }
    
    right_column = this->routes[offset].dest;

    (*line) = this->FormatString(left_column, right_column);
  }
  else {
    (*line).clear();
  }
}

void RoutePage::Update() {

  unsigned int current_page = ceil(float(this->offset) / 5) + 1;
  unsigned int total_pages = floor(float(this->routes.size()) / 5) + 1;

  this->Clear();

  this->heading = this->GenerateHeading("Route", current_page, total_pages);

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
  case BUTTON_UP:
    if(static_cast<int>(this->offset) - 5 > 0) {
      this->offset = this->offset - 5;
    } else {
      this->offset = 0;
    }
    return;
  case BUTTON_DOWN:
    if(this->offset + 5 <= this->routes.size()) {
      this->offset = this->offset + 5;
    }
    return;
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

  if(!this->delete_mode) {
    if(airway_index >= 0) {
      unsigned int operation_index = this->offset + airway_index;
      if(operation_index < this->routes.size()) {
        /* Todo */
      }
      else {
        RouteItem item;
        item.airway = this->input;
        this->routes.push_back(item);
        this->input.clear();
      }
    }
    else if(dest_index >= 0) {
      unsigned int operation_index = this->offset + dest_index;

      if(operation_index >= this->routes.size()) {
        RouteItem item;

        this->routes.push_back(item);
        
        operation_index = this->routes.size() - 1;
      }

      std::string source = "";
      if(this->routes.size() > 1) {
        source = this->routes[operation_index-1].dest;
      }

      std::string airway = this->routes[operation_index].airway;
      std::string dest = this->input;

      std::vector<NavAidInfo>* flightplan = &this->flight->flightplan;
      
      if(airway.size() > 0) {
        std::vector<NavAidInfo> waypoints;

        this->airway_reader.FindSegment(airway, source, dest, waypoints);
      
        if(waypoints.size() == 0) {
          this->error = "Airway not found";
          return;
        }
        
        if(this->routes[operation_index].inserted) {
          RouteItem item;
          item = this->routes[operation_index];
          
          this->flight->flightplan.erase(this->flight->flightplan.begin() + item.start_index, this->flight->flightplan.begin() + item.end_index + 1);
        }
        
        this->routes[operation_index].dest = this->input;
        
        unsigned int insert_index = this->routes[operation_index-1].end_index;
                
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
      }
      else {
        unsigned int insert_index = 0;
        if(this->routes.size() > 1) { 
          insert_index = this->routes[operation_index-1].end_index;
        }

        /* Clear navaids storage to ensure consistency */
        this->flight->temp_navaids.clear();

        Navigation::FindNavAid(this->input, this->flight->temp_navaids);

        if(this->flight->temp_navaids.size() == 0) {
          this->error = "Navaid could not be found";
          return;
        }
        
        if(this->flight->temp_navaids.size() > 1) {
          this->flight->temp_navaid_insert = insert_index;
          this->input.clear();
          pages->SwitchPage("navaid");
          return;
        }
        
        if(insert_index + 1 <= (*flightplan).size()) {
          (*flightplan).insert((*flightplan).begin() + insert_index + 1,
                               this->flight->temp_navaids[0]);
        } else {
          (*flightplan).push_back(this->flight->temp_navaids[0]);
        }

      }
      this->input.clear();
      this->GenerateRoutes();
    }
  }
  else {
    unsigned int operation_index = this->offset + airway_index;

    if(airway_index >= 0 && operation_index < this->routes.size()) {

      RouteItem item = this->routes[operation_index];

      if(item.inserted) {
        this->flight->flightplan.erase(this->flight->flightplan.begin() + item.start_index, this->flight->flightplan.begin() + item.end_index + 1);
        this->GenerateRoutes();
      }
      else {
        this->routes.erase(this->routes.begin() + operation_index);
      }
    }
  }
}

bool RoutePage::HandleDelete() {
  bool action = Page::HandleDelete();
  if(!action) {
    this->delete_mode = !this->delete_mode;
  }
  return true;
}

bool RoutePage::OnSwitch() {
  this->GenerateRoutes();
  return true;
}

std::string RoutePage::GetStatus() {
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

void RoutePage::GenerateRoutes() {
  this->routes.clear();
  std::vector<NavAidInfo>* flightplan = &this->flight->flightplan;
  
  for(size_t i=0; i < flightplan->size();) {
    if((*flightplan)[i].fmc_sid) {
      RouteItem item;
      
      item.airway = "";
      item.dest = (*flightplan)[i].id;
      item.start_index = i;
      item.end_index = i;
      item.sid = true;
      item.inserted = true;

      for(i++; i < flightplan->size(); i++) {
        if((*flightplan)[i].fmc_sid) {
          item.dest = (*flightplan)[i].id;
          item.end_index = i;
        }
        else {
          break;
        }
      }
      this->routes.push_back(item);

    }
    else if((*flightplan)[i].fmc_star) {
      RouteItem item;

      item.airway = "";
      item.dest = (*flightplan)[i].id;
      item.start_index = i;
      item.end_index = i;
      item.star = true;
      item.inserted = true;
      
      for(i++; i < flightplan->size(); i++) {
        if((*flightplan)[i].fmc_star) {
          item.dest = (*flightplan)[i].id;
          item.end_index = i;
        }
        else {
          break;
        }
      }

      this->routes.push_back(item);
    }    
    else if((*flightplan)[i].fmc_airway.size() > 0) {
      RouteItem item;
      int airway_start = i;

      item.airway = (*flightplan)[airway_start].fmc_airway;
      item.dest = (*flightplan)[i].id;
      item.start_index = i;
      item.end_index = i;
      item.inserted = true;

      for(i++; i < flightplan->size(); i++) {
        if((*flightplan)[airway_start].fmc_airway == (*flightplan)[i].fmc_airway) {
          item.dest = (*flightplan)[i].id;
          item.end_index = i;
        }
        else {
          break;
        }
      }

      this->routes.push_back(item);
    }
    else if((*flightplan)[i].fmc_airway.size() == 0) {
      RouteItem item;
      item.airway = "";
      item.dest = (*flightplan)[i].id;
      item.start_index = i;
      item.end_index = i;
      item.inserted = true;
      this->routes.push_back(item);
      i++;
    }
  }
}
