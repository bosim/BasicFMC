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

#ifndef PAGE_AIRPORT_H
#define PAGE_AIRPORT_H

#include <string>

#include "Page.h"
#include "Flight.h"

class DepArrPage : public Page {
 public:
  DepArrPage(Flight* flight) : Page(flight), offset(0), selected_runway(-1) { }
  void PrintLine(unsigned int offset, std::string* line, std::vector<std::string>* runways, std::vector<std::string>* procedures);
  void Update();
  void HandleSK(int key);
  virtual bool OnSwitch();
 protected:
  unsigned int offset;
  unsigned int selected_runway;
  bool sid;
  std::vector<std::string>* runways;
  std::vector<Procedure>* procedures;
  std::vector<std::string> procedures_labels;
};

class DepPage : public DepArrPage {
 public:
  DepPage(Flight* flight);
  void Update();
};

class ArrPage : public DepArrPage {
 public:
  ArrPage(Flight* flight);
  void Update();
};

class AirportPage : public Page {

 public:
  AirportPage(Flight* flight);
  void Update();
  void HandleSK(int key);
  bool OnSwitch();
};

#endif
