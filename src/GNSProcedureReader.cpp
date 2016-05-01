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

#include "GNSProcedureReader.h"

void GNSProcedureReader::ReadProcedures(std::string Filename, std::vector<Procedure>& procedures, bool star) {
  std::ifstream filehandle(Filename.c_str());
  std::string line;
  std::vector<std::string> elements;
  Procedure procedure;
  ProcedureWaypoint waypoint;
  bool got_procedure = false;
    
  if(filehandle.is_open()) {
    while(getline(filehandle, line)) {
      elements.clear();
      
      SplitLine(line, elements, ',');
      
      if(elements.size() > 1) {
        if((!star && elements[0] == "SID") || (star && elements[0] == "STAR")) {
          procedure = Procedure();
          
          procedure.name = elements[1];
          procedure.runway = elements[2];
          procedure.star = elements[0] == "STAR";

          procedures.push_back(procedure);

          got_procedure = true;
        }
        else if(got_procedure && procedures.size() > 0 &&
                (elements[0] == "DF" || elements[0] == "TF" ||
                 elements[0] == "CF" || elements[0] == "IF")) {
          waypoint = ProcedureWaypoint();

          waypoint.id = elements[1];
          waypoint.lat = atof(elements[2].c_str());
          waypoint.lon = atof(elements[3].c_str());
          waypoint.speed = 0;
          waypoint.altitude = 0;
          
          procedures[procedures.size()-1].waypoints.push_back(waypoint);
        }
      }
      else {
        got_procedure = false;
      }
    }
  }
}

void GNSProcedureReader::ReadSidFile(std::string Filename, std::vector<Procedure>& procedures) {
  this->ReadProcedures(Filename, procedures, false);
}

void GNSProcedureReader::ReadStarFile(std::string Filename, std::vector<Procedure>& procedures) {
  this->ReadProcedures(Filename, procedures, true);
}
