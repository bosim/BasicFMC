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

#include "Page_Progress.h"
#include "Utils.h"

#include <sstream>
#include <iomanip>

ProgressPage::ProgressPage(Flight* flight) : Page(flight) {

  this->heading = this->FormatString(std::string("Progress"),
                                     std::string("1/1"));
  this->line1_h = this->FormatString(std::string("Next waypoint"),
                                     std::string(""));
  this->line2_h = this->FormatString(std::string("Next waypoint after"),
                                     std::string(""));
  this->line3_h = this->FormatString(std::string("Destination"),
                                     std::string(""));
}

void ProgressPage::Update() {

  std::stringstream ss;
  
  if(this->flight->flightplan.size() > 0) {
    ss.str("");
    ss << std::fixed << std::setprecision(1)
       << this->flight->flightplan[0].fmc_distance;

    this->line1 = this->FormatString(this->flight->flightplan[0].id,
                                     ss.str());
  }
  if(this->flight->flightplan.size() > 1) {
    ss.str("");
    ss << std::fixed << std::setprecision(1)
       << this->flight->flightplan[1].fmc_distance;

    this->line2 = this->FormatString(this->flight->flightplan[1].id,
                                     ss.str());
  }

  if(this->flight->flightplan.size() > 0) {
    NavAidInfo* last_waypoint = &this->flight->flightplan[this->flight->flightplan.size()-1];
    NavAidInfo* dest_airport = &this->flight->dest_airport;
    double distance_to_last = last_waypoint->fmc_distance;
    double distance_to_airport = distance(last_waypoint->lat, last_waypoint->lon,
                                          dest_airport->lat, dest_airport->lon);
    double distance_to_dest = distance_to_last + distance_to_airport;
 
    ss.str("");
    ss << std::fixed << std::setprecision(1) << distance_to_dest;
    this->line3 = this->FormatString(dest_airport->id,
                                     ss.str());
    
  }
  
  this->Draw();
}

void ProgressPage::HandleSK(int key) {
}
