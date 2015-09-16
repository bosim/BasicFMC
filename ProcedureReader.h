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
  ProcedureWaypoint();

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

  Procedure();
  void dump();
};
  

class ProcedureReader {
public:
  Procedure ReadProcedureLine(std::vector<std::string> const& elements, bool star);

  void ReadSidWaypoint(std::vector<std::string> const& elements,
                       Procedure& procedure, size_t offset);
  void ReadSidFile(std::string Filename, std::vector<Procedure>& procedures);

  void ReadStarWaypoint(std::vector<std::string> const& elements,
                        Procedure& procedure, size_t offset);
  void ReadStarFile(std::string Filename, std::vector<Procedure>& procedures);
  
  void GetRunways(std::vector<Procedure>& procedures, std::vector<std::string>& result); 
};

#endif

