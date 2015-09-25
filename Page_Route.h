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

#ifndef PAGE_ROUTE_H
#define PAGE_ROUTE_H

#include <string>

#include "Page.h"
#include "Flight.h"
#include "AirwayReader.h"

class RouteItem {
 public:
  RouteItem() : airway(), dest(), start_index(0), end_index(0),
    inserted(false), sid(false), star(false) { }
  std::string airway;
  std::string dest;
  unsigned int start_index;
  unsigned int end_index;
  bool inserted;
  bool sid;
  bool star;
};

class RoutePage : public Page {

 public:
  RoutePage(Flight* flight);
  void Update();
  void HandleSK(int key);
  bool HandleDelete();
  bool OnSwitch();
  std::string GetStatus();
 private:
  void PrintLine(unsigned int offset, std::string* line);
  void GenerateRoutes();
  std::vector<RouteItem> routes;
  unsigned int offset;
  bool delete_mode;
  AirwayReader airway_reader;  
};

#endif
