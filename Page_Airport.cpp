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

#include "Page_Airport.h"

#include "Pages.h"

extern Pages* pages;

void DepArrPage::PrintLine(unsigned int offset, std::string* line, std::vector<std::string>* runways, std::vector<std::string>* procedures) {
  std::string left_column = "";
  std::string right_column = "";

  if(offset < (*runways).size()) {
    left_column = (*runways)[offset];

    if(offset == this->selected_runway) {
      left_column += "*";
    }
  }
  if(offset < (*procedures).size()) {
    right_column = (*procedures)[offset];
  }

  if(left_column.length() > 0 || right_column.length() > 0) {
    (*line) = this->FormatString(left_column, right_column);
  }
  else {
    (*line).clear();
  }
}

void DepArrPage::Update() {
  std::vector<std::string>* runways = this->runways;
  std::vector<std::string>* procedures = &this->procedures_labels;

  this->PrintLine(this->offset, &this->line1, runways, procedures);
  this->PrintLine(this->offset + 1, &this->line2, runways, procedures);
  this->PrintLine(this->offset + 2, &this->line3, runways, procedures);
  this->PrintLine(this->offset + 3, &this->line4, runways, procedures);
  this->PrintLine(this->offset + 4, &this->line5, runways, procedures);

  this->Draw();

}

void DepArrPage::HandleSK(int key) {
  int runway_index = -1;
  int procedure_index = -1;
  
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
    if(this->offset + 5 < std::max((*this->runways).size(),
                                   this->procedures_labels.size())) {
      this->offset = this->offset + 5;
    }
    return;
  case LSK1: runway_index = 0; break;
  case LSK2: runway_index = 1; break;
  case LSK3: runway_index = 2; break;
  case LSK4: runway_index = 3; break;
  case LSK5: runway_index = 4; break;
  //case LSK6: runway_index = 5; break;
  case RSK1: procedure_index = 0; break;
  case RSK2: procedure_index = 1; break;
  case RSK3: procedure_index = 2; break;
  case RSK4: procedure_index = 3; break;
  case RSK5: procedure_index = 4; break;
  //case RSK6: procedure_index = 5; break;
  default:
    return;
  }

  if(this->selected_runway >= 0 && procedure_index >= 0) {
    unsigned int selected_procedure = this->offset + procedure_index;
    std::string procedure_label = this->procedures_labels[selected_procedure];
    std::string runway = (*this->runways)[this->selected_runway];

    Procedure* found_procedure = NULL;

    for(size_t i=0; i < (*this->procedures).size(); i++) {
      Procedure* procedure = &(*this->procedures)[i];
      if(procedure->name == procedure_label && procedure->runway == runway) {
        found_procedure = procedure;
      }
    }

    if(found_procedure != NULL) {
      int count = 0;
      int start = -1;
      int end = -1;
      
      /* Clean existing procedures */
      if(this->flight->flightplan.size() > 0) {
        for(size_t i=0; i < this->flight->flightplan.size(); i++) {
          if((this->sid && this->flight->flightplan[i].fmc_sid) ||
             (!this->sid && this->flight->flightplan[i].fmc_star)) {
          
            start = i;
            break;
          }
        }

        for(size_t i=this->flight->flightplan.size() - 1; i >= 0; i--) {
          if((this->sid && this->flight->flightplan[i].fmc_sid) ||
             (!this->sid && this->flight->flightplan[i].fmc_star)) {
            
            end = i;
            break;
          }
        }
        
        if(start >= 0 && end >= 0) {
          this->flight->flightplan.erase(this->flight->flightplan.begin() + start,
                                         this->flight->flightplan.begin() + end + 1);
        }
      }
      
      for(size_t i=0; i < (*found_procedure).waypoints.size(); i++) {
        ProcedureWaypoint waypoint = (*found_procedure).waypoints[i];
        NavAidInfo navaid_info;
        float lat, lon;

        /* IF procedure provides lat/lon, it is fine */
        if(waypoint.lon && waypoint.lat) {
          lon = waypoint.lon;
          lat = waypoint.lat;
        }
        else {
          /* Otherwise we will use dep/dest airport to find the nearest 
             waypoint in the database */
          if(this->sid) {
            lon = this->flight->dep_airport.lon;
            lat = this->flight->dep_airport.lat;
          }
          else {
            lon = this->flight->dest_airport.lon;
            lat = this->flight->dest_airport.lat;
          }
        }

        try {
          navaid_info = Navigation::FindNavAidIdLonLat(waypoint.id, lon, lat);
        }
        catch(NavAidNotFoundException e) {
          navaid_info = NavAidInfo();
          navaid_info.ref = 0;
          navaid_info.type = -1;
          navaid_info.lat = waypoint.lat;
          navaid_info.lon = waypoint.lon;
          navaid_info.id = waypoint.id;
          navaid_info.name = waypoint.id;
        } 

        if(waypoint.speed) {
          navaid_info.fmc_forced_speed = waypoint.speed;
        }

        if(waypoint.altitude) {
          navaid_info.fmc_forced_altitude = waypoint.altitude;
        }

        if(navaid_info.lat && navaid_info.lon) {
          if(this->sid) {
            navaid_info.fmc_sid = true;
            this->flight->flightplan.insert(this->flight->flightplan.begin() + count, navaid_info);
            count++;
          } else {
            navaid_info.fmc_star = true;
            this->flight->flightplan.push_back(navaid_info);
          }
        }
        else {
          XPLMDebugString("Could not find ");
          XPLMDebugString(navaid_info.id.c_str());
          XPLMDebugString("\n");
        }
      }
      this->flight->SyncToXPFMC();

      pages->SwitchPage("legs");
    }
  }
  else if(runway_index >= 0) {
    procedures_labels.clear();

    this->selected_runway = this->offset + runway_index;
    std::string runway = (*this->runways)[this->selected_runway];

    for(size_t i=0; i < (*this->procedures).size(); i++) {
      Procedure* procedure = &(*this->procedures)[i];
      if(procedure->runway == runway) {
        this->procedures_labels.push_back(procedure->name);
      }
    }
  }

}

bool DepArrPage::OnSwitch() {  
  this->procedures_labels.clear();
  this->selected_runway = -1;
  return true;
}

DepPage::DepPage(Flight* flight) : DepArrPage(flight) {
  this->sid = true;
  this->runways = &this->flight->sids_runways;
  this->procedures = &this->flight->sids;
}

void DepPage::Update() {
  this->heading = this->FormatString(std::string("Departures"),
                                     std::string("1/1"));
  DepArrPage::Update();
}

ArrPage::ArrPage(Flight* flight) : DepArrPage(flight) {
  this->sid = false;
  this->runways = &this->flight->stars_runways;
  this->procedures = &this->flight->stars;
}

void ArrPage::Update() {
  this->heading = this->FormatString(std::string("Arrivals"),
                                     std::string("1/1"));
  DepArrPage::Update();
}

AirportPage::AirportPage(Flight* flight) : Page(flight) {
}

void AirportPage::Update() {
  this->Clear();

  this->heading = this->FormatString(std::string("Airport"),
                                     std::string("1/1"));

  this->line1 = this->FormatString(this->flight->dep_airport.id,
                                     std::string("DEP >"));
  this->line2 = this->FormatString(this->flight->dest_airport.id,
                                     std::string("ARR >"));

  this->Draw();
}

void AirportPage::HandleSK(int key) {
  switch(key) {
  case RSK1:
    if(this->flight->sids.size() > 0) {
      pages->SwitchPage("airport_dep");
    }
    else {
      this->error = "No SIDS";
    }
    break;
  case RSK2:
    if(this->flight->stars.size() > 0) {
      pages->SwitchPage("airport_arr");
    }
    else {
      this->error = "No STARS";
    }
    break;
  }
}

bool AirportPage::OnSwitch() {
  if(this->flight->dep_airport.isEmpty() ||
     this->flight->dest_airport.isEmpty()) {
    return false;
  }

  return true;
}
