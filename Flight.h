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

#ifndef FLIGHT_H
#define FLIGHT_H

#include "Navigation.h"

#include <vector>

class Flight {

 public:
  NavAidInfo dep_airport;
  NavAidInfo dest_airport;
  std::string flightno;
  std::vector<NavAidInfo> flightplan;
  
  void SetDepAirport(std::string Airport) {
    this->dep_airport = Navigation::FindAirport(Airport);
  }
  void SetDestAirport(std::string Airport) {
    this->dest_airport = Navigation::FindAirport(Airport);
  }

  void SyncToXPFMC() {
    for(int i=0; i < XPLMCountFMSEntries(); i++) {
      XPLMClearFMSEntry(i);
    }
    for(unsigned int i=0; i < this->flightplan.size(); i++) {
      XPLMSetFMSEntryInfo(i, this->flightplan[i].ref, 0);
    }
  }
};

#endif
