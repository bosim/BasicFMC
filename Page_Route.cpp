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

RoutePage::RoutePage(Flight* flight) : Page(flight) {

}

void RoutePage::Update() {  
  this->Draw();
}

void RoutePage::HandleSK(int key) {
  switch(key) {
    case LSK1:
      break;
    case RSK1:
      break;
    case RSK2:
      break;
  }
}
