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

#include "ProcedureReader.h"
#include "FreeNavProcedureReader.h"

#include <iostream>
#include <fstream>

ProcedureWaypoint::ProcedureWaypoint() : id(""), lon(0), lat(0) {
}

Procedure::Procedure() : runway(""), name(""), star(false), waypoints() {
}

void Procedure::dump() {
  std::cout << this->runway << "," << this->name << std::endl;
  for(size_t i=0; i < waypoints.size(); i++) {
    std::cout << waypoints[i].id << " " <<
      waypoints[i].lon << "," <<
      waypoints[i].lat << " Speed " <<
      waypoints[i].speed << " Altitude " <<
      waypoints[i].altitude << std::endl;
  }
}

void ProcedureReader::ReadSidFile(std::string Airport, std::vector<Procedure>& procedures) {
  FreeNavProcedureReader freenav_pr;

  std::string freenav_filename = GetProcedureFilename(Airport);
  std::ifstream freenav_handle(freenav_filename.c_str());
  if(freenav_handle.good()) {
    freenav_pr.ReadSidFile(freenav_filename, procedures);
  }
}

void ProcedureReader::ReadStarFile(std::string Airport, std::vector<Procedure>& procedures) {
  FreeNavProcedureReader freenav_pr;

  std::string freenav_filename = GetProcedureFilename(Airport, true);
  std::ifstream freenav_handle(freenav_filename.c_str());
  if(freenav_handle.good()) {
    freenav_pr.ReadStarFile(freenav_filename, procedures);
  }
}


void ProcedureReader::GetRunways(std::vector<Procedure>& procedures, std::vector<std::string>& result) {
  std::set<std::string> set;

  for(size_t i=0; i < procedures.size(); i++) {
    if(set.find(procedures[i].runway) == set.end()) {
      set.insert(procedures[i].runway);
    }
  }
  
  for(std::set<std::string>::iterator it = set.begin();
      it != set.end();
      ++it) {
    result.push_back(*it);
  }
}


