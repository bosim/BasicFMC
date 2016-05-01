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

#include "FreeNavProcedureReader.h"

Procedure FreeNavProcedureReader::ReadProcedureLine(std::vector<std::string> const& elements, bool star) {
  Procedure procedure;
  
  procedure.runway = elements[0];
  procedure.name = elements[1];
  procedure.star = star;

  return procedure;
}

void FreeNavProcedureReader::ReadSidWaypoint(std::vector<std::string> const& elements,
                                      Procedure& procedure, size_t offset) {
  ProcedureWaypoint waypoint;

  if(offset + 6 < elements.size()) {
    waypoint.id = elements[offset+4];
    waypoint.lat = atof(elements[offset+5].c_str());
    waypoint.lon = atof(elements[offset+6].c_str());
    waypoint.altitude = 0;
    waypoint.speed = 0;

    procedure.waypoints.push_back(waypoint);
  }
}

void FreeNavProcedureReader::ReadSidFile(std::string Filename, std::vector<Procedure>& procedures) {
  std::ifstream filehandle(Filename.c_str());
  std::string line;
    
  if(filehandle.is_open()) {
    while(getline(filehandle, line)) {
      std::vector<std::string> elements;
      SplitLine(line, elements, ',');

      if(elements.size() > 1) {

        Procedure procedure = this->ReadProcedureLine(elements, false);
        
        for(size_t i=10; i < elements.size(); i = i+7) {
          this->ReadSidWaypoint(elements, procedure, i);
        }

        procedures.push_back(procedure);
      }
    }
  }
}

void FreeNavProcedureReader::ReadStarWaypoint(std::vector<std::string> const& elements,
                                       Procedure& procedure, size_t offset) {
  ProcedureWaypoint waypoint;

  if(offset + 9 < elements.size()) {
  
    waypoint.id = elements[offset+4];
    waypoint.lat = atof(elements[offset+5].c_str());
    waypoint.lon = atof(elements[offset+6].c_str());
    waypoint.altitude = atoi(elements[offset+8].c_str());
    waypoint.speed = atoi(elements[offset+9].c_str());

    procedure.waypoints.push_back(waypoint);
  }
}

void FreeNavProcedureReader::ReadStarFile(std::string Filename, std::vector<Procedure>& procedures) {
  std::ifstream filehandle(Filename.c_str());
  std::string line;
    
  if(filehandle.is_open()) {
    while(getline(filehandle, line)) {
      std::vector<std::string> elements;
      SplitLine(line, elements, ',');
      
      if(elements.size() > 1) {
        
        Procedure procedure = this->ReadProcedureLine(elements, true);
        
        for(size_t i=4; i < elements.size(); i = i+15) {
          this->ReadStarWaypoint(elements, procedure, i);
        }
        
        procedures.push_back(procedure);
      }
    }
  }
}

