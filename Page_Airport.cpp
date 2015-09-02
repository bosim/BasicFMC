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

AirportPage::AirportPage(Flight* flight) : Page(flight), mode(MODE_OVERVIEW) {
}

void AirportPage::Update() {
  this->heading = this->FormatString(std::string("Airport"),
                                     std::string("1/1"));

  if(this->mode == MODE_OVERVIEW) {
    
    this->line1_h = this->FormatString(this->flight->dep_airport.id,
                                     std::string("DEP >"));
    this->line2_h = this->FormatString(this->flight->dest_airport.id,
                                     std::string("ARR >"));
  }
  else {

  }
  this->Draw();
}

void AirportPage::HandleSK(int key) {
  switch(key) {
  case RSK1:
    this->sid = true;
    break;
  case RSK2:
    this->sid = false;
    break;
  }
}
