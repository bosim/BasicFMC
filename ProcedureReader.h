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

#ifndef PROCEDUREREADER_H
#define PROCEDUREREADER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <set>

#include "Utils.h"

class ProcedureWaypoint {
public:
  ProcedureWaypoint() : id(""), lon(0), lat(0) { }
  std::string id;
  float lon;
  float lat;
  int speed;
  int altitude;
};

class Procedure {
public:
  std::string runway;
  std::string name;
  bool star;
  std::vector<ProcedureWaypoint> waypoints;

  void dump() {
    std::cout << this->runway << "," << this->name << std::endl;
    for(size_t i=0; i < waypoints.size(); i++) {
      std::cout << waypoints[i].id << " " <<
        waypoints[i].lon << "," <<
        waypoints[i].lat << " Speed " <<
        waypoints[i].speed << " Altitude " <<
        waypoints[i].altitude << std::endl;
    }
  }
};
  

class ProcedureReader {
public:
  void ReadSidFile(std::string Filename, std::vector<Procedure>& procedures) {
    std::ifstream filehandle(Filename.c_str());
    std::string line;
    
    if(filehandle.is_open()) {
      while(getline(filehandle, line)) {
        std::vector<std::string> elements;
        SplitLine(line, elements, ',');

        if(elements.size() > 1) {
          Procedure procedure;

          procedure.runway = elements[0];
          procedure.name = elements[1];
          procedure.star = false;

          for(size_t i=10; i < elements.size(); i = i+7) {
            ProcedureWaypoint waypoint;

            waypoint.id = elements[i+4];
            waypoint.lat = atof(elements[i+5].c_str());
            waypoint.lon = atof(elements[i+6].c_str());
            waypoint.altitude = 0;
            waypoint.speed = 0;
            procedure.waypoints.push_back(waypoint);
          }
          procedures.push_back(procedure);
        }
      }
    }
  }
  void ReadStarFile(std::string Filename, std::vector<Procedure>& procedures) {
    std::ifstream filehandle(Filename.c_str());
    std::string line;
    
    if(filehandle.is_open()) {
      while(getline(filehandle, line)) {
        std::vector<std::string> elements;
        SplitLine(line, elements, ',');

        if(elements.size() > 1) {
          Procedure procedure;

          procedure.runway = elements[0];
          procedure.name = elements[1];
          procedure.star = true;

          for(size_t i=4; i < elements.size(); i = i+15) {
            ProcedureWaypoint waypoint;

            waypoint.id = elements[i+4];
            waypoint.lat = atof(elements[i+5].c_str());
            waypoint.lon = atof(elements[i+6].c_str());
            waypoint.altitude = atoi(elements[i+8].c_str());
            waypoint.speed = atoi(elements[i+9].c_str());
            procedure.waypoints.push_back(waypoint);
          }
          procedures.push_back(procedure);
        }
      }
    }
  }

  void GetRunways(std::vector<Procedure>& procedures, std::vector<std::string>& result) {
    std::set<std::string> set;

    for(size_t i=0; i < procedures.size(); i++) {
      if(set.find(procedures[i].runway) == set.end()) {
        set.insert(procedures[i].runway);
      }
    }

    for(auto it = set.begin(); it != set.end(); ++it) {
      result.push_back(*it);
    }
  }
};

#endif

#if 0
int main() {
  ProcedureReader p;
  std::vector<Procedure> result;
  p.ReadSidFile("X-Plane 10/Resources/plugins/BS-FMC/FD_FMC/EKCH/SID_data.csv", result);
  result.clear();
  p.ReadStarFile("X-Plane 10/Resources/plugins/BS-FMC/FD_FMC/EKCH/STAR_data.csv", result);
  auto s = p.GetRunways(result);
  for(auto it=s.begin(); it != s.end(); it++) {
    std::cout << *it << std::endl;
  }
}
#endif
