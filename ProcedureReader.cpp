#include "ProcedureReader.h"

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

Procedure ProcedureReader::ReadProcedureLine(std::vector<std::string> const& elements, bool star) {
  Procedure procedure;
  
  procedure.runway = elements[0];
  procedure.name = elements[1];
  procedure.star = star;

  return procedure;
}

void ProcedureReader::ReadSidWaypoint(std::vector<std::string> const& elements,
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

void ProcedureReader::ReadSidFile(std::string Filename, std::vector<Procedure>& procedures) {
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

void ProcedureReader::ReadStarWaypoint(std::vector<std::string> const& elements,
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

void ProcedureReader::ReadStarFile(std::string Filename, std::vector<Procedure>& procedures) {
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


