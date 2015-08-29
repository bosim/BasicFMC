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

#include "Page.h"
#include "Flight.h"

class LegsPage : public Page {

 public:
  LegsPage(Flight* flight);
  void PrintLine(unsigned int offset, std::string* line,
                 std::vector<NavAidInfo>* flightplan);
  void Update();
  void HandleSK(int key);
  bool HandleDelete();
  std::string GetStatus();
 private:
  unsigned int offset;
  bool delete_mode;
};
