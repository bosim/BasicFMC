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

#include "Page_Init.h"

InitPage::InitPage(Flight* flight) : Page(flight) {

  this->heading = this->FormatString(std::string("Init"), std::string("1/1"));
  this->line1_h = this->FormatString("Dep Airport", "Dest Airport");
  this->line2_h = this->FormatString("", "Flight No");
}

void InitPage::Update() {
  std::string dep_airport = this->flight->dep_airport.length() > 0 ?
    this->flight->dep_airport : std::string("----");
  std::string dest_airport = this->flight->dest_airport.length() > 0 ?
    this->flight->dest_airport : std::string("----");
  std::string flightno = this->flight->flightno.length() > 0 ?
    this->flight->flightno : std::string("----");
  
  this->line1 = this->FormatString(dep_airport, dest_airport);
  this->line2 = this->FormatString("", flightno);
  
  this->Draw();
}

void InitPage::HandleSK(int key) {
    switch(key) {
    case LSK1:
      this->flight->dep_airport = this->input;
      this->input.clear();
      break;
    case RSK1:
      this->flight->dest_airport = this->input;
      this->input.clear();
      break;
    case RSK2:
      this->flight->flightno = this->input;
      this->input.clear();
      break;
    }
  }
