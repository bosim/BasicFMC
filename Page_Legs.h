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

const int MODE_LEGS = 0;
const int MODE_NAVAID = 1;

class LegsPage : public Page {

 public:
  LegsPage(Flight* flight);
  void PrintLine(unsigned int offset, std::string* line,
                 std::vector<NavAidInfo>* flightplan,
                 bool coordinates);
  void Update();
  void HandleSK(int key);
  bool HandleDelete();
  std::string GetStatus();
 protected:
  void LegsHandleSK(int key);
  void LegsUpdate();
  bool LegsHandleDelete();
  void NavaidUpdate();
  void NavaidHandleSK(int key);
  bool NavaidHandleDelete();
 private:
  unsigned int offset;
  unsigned int navaid_offset;
  unsigned int operation_index;
  bool delete_mode;
  int mode;
  std::vector<NavAidInfo> navaids;
};
