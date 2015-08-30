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

#ifndef AIRWAYREADER_H
#define AIRWAYREADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <set>
#include <algorithm>
#include <map>
#include <cassert>

class AirwaySegment {
public:
  std::string source;
  float source_lon;
  float source_lat;
  std::string dest;
  float dest_lon;
  float dest_lat;
  bool upper;
  int lower_level;
  int higher_level;
  std::string name;

  AirwaySegment() { }
  AirwaySegment(std::string source, float source_lon, float source_lat, std::string dest, float dest_lon, float dest_lat,
                bool upper, int lower_level, int higher_level, std::string name) : 
    source(source), source_lon(source_lon), source_lat(source_lat), dest(dest), dest_lon(dest_lon), dest_lat(dest_lat), upper(upper),
    lower_level(lower_level), higher_level(higher_level), name(name) {

  }

  void dump() {
    std::cout << "Source " << this->source << "(" << this->source_lon << "," << this->source_lat << ")";
    std::cout << "Dest " << this->dest << "(" << this->dest_lon << "," << this->dest_lat << ")";
    std::cout << "Upper " << this->upper << " " << this->lower_level << " " << this->higher_level << " " << this->name << std::endl;
  }
};

class AirwayReader {
public:
  AirwayReader(std::string Filename) : Filename(Filename) {
  }

  void FindSegment(std::string airway, std::string source, std::string dest, std::vector<AirwaySegment>& result) {
    std::ifstream filehandle(this->Filename.c_str());
    std::string line;
    std::vector<AirwaySegment> temp_result;

    if(filehandle.is_open()) {
      while(getline(filehandle, line)) {

        if(line.find(airway) == std::string::npos)
          continue;
        
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string buf;

        while (ss >> buf) {
          if(buf != "") {
            tokens.push_back(buf);
          }
        }

        if(tokens.size() != 10)
          continue;
        
        AirwaySegment segment = AirwaySegment(tokens[0], atof(tokens[1].c_str()), atof(tokens[2].c_str()),
                                              tokens[3], atof(tokens[4].c_str()), atof(tokens[5].c_str()),
                                              tokens[6] == "1" ? true : false, atoi(tokens[7].c_str()),
                                              atoi(tokens[8].c_str()), tokens[9]);
        if(segment.name == airway) {
            temp_result.push_back(segment);
        }
      }
    }

    std::vector<std::pair<std::string, int> > paths;
    for(int i=0; i < temp_result.size(); ++i) {
      if(temp_result[i].source == source) {
        paths.push_back(std::pair<std::string, int>(temp_result[i].dest, i));
      }
      else if(temp_result[i].dest == source) {
        paths.push_back(std::pair<std::string, int>(temp_result[i].source, i));
      }
    }

    for(int i=0; i < paths.size(); ++i) {
      std::vector<AirwaySegment> found_result;
      std::string tmp = paths[i].first;
      found_result.push_back(temp_result[paths[i].second]);
      std::set<std::string> visited;
      visited.insert(source);
      bool found = true;
      while(found) {
        found = false;
        for(int j=0; j < temp_result.size(); j++) {
          if(temp_result[j].source == tmp && visited.find(temp_result[j].dest) == visited.end()) {
            visited.insert(tmp);
            tmp = temp_result[j].dest;
            found_result.push_back(temp_result[j]);
            found = true;
          }
          else if(temp_result[j].dest == tmp && visited.find(temp_result[j].source) == visited.end()) {
            visited.insert(tmp);
            tmp = temp_result[j].source;
            found_result.push_back(temp_result[j]);
            found = true;
          }
        }
      }

      int q;

      for(q=0; q < found_result.size(); q++) {
        if((q == 0 && found_result[q].source != source) ||
           (q > 0 && found_result[q-1].dest != found_result[q].source)) {
          std::swap(found_result[q].source, found_result[q].dest);
          std::swap(found_result[q].source_lon, found_result[q].dest_lon);
          std::swap(found_result[q].source_lat, found_result[q].dest_lat);
        }
        if(found_result[q].dest == dest) {
          break;
        }
      }

      if(q < found_result.size()) {
        result.resize(q+1);
        std::copy(found_result.begin(), found_result.begin() + q + 1, result.begin());
        return;
      }
    }
  }
private:
  std::vector<AirwaySegment> segments;
  std::string Filename;
};

int main() {
  AirwayReader a("X-Plane 10/Resources/default data/earth_awy.dat");
  std::vector<AirwaySegment> result;
}

#endif

