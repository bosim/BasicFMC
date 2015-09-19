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

#include <cmath> 

#include "XPLMDefs.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"

#include "Utils.h"

std::string GetPluginDir() {
  char buf[256];
  std::string PluginDir;
  std::string Delimiter = std::string(XPLMGetDirectorySeparator());
  
  XPLMGetSystemPath(buf);
  PluginDir = std::string(buf) + "Resources" + Delimiter + "plugins" + Delimiter + "BasicFMC" + Delimiter;

  return PluginDir;
}

std::string GetAirwayFilename(bool custom) {
  char buf[256];
  std::string AwyFile;
  std::string Delimiter = std::string(XPLMGetDirectorySeparator());
  
  XPLMGetSystemPath(buf);
  if(custom) {
    AwyFile = std::string(buf) + "Custom Data" + Delimiter + "earth_awy.dat";
  }
  else {
    AwyFile = std::string(buf) + "Resources" + Delimiter + "default data" + Delimiter + "earth_awy.dat";
  }
  return AwyFile;
}

std::string GetProcedureFilename(std::string Airport, bool star) {
  std::string PluginDir = GetPluginDir();
  std::string Delimiter = std::string(XPLMGetDirectorySeparator());
  std::string ProcedureFilename = PluginDir + "FD_FMC" + Delimiter + Airport + Delimiter + (star ? "STAR_data.csv" : "SID_data.csv");

  return ProcedureFilename;
}

int CoordInRect(float x, float y, float l, float t, float r, float b) {
  return ((x >= l) && (x < r) && (y < t) && (y >= b));
}

void SplitLine(std::string s, std::vector<std::string>& l, char delim, size_t times) {
  std::string::size_type i = 0;
  std::string::size_type last = 0;
  
  bool ignore = false;

  while(i < s.size()) {
    if(s[i] == '"') {

      if(!ignore) {
	ignore = true;
	last = i + 1;
      }
      else {
        std::string new_str = s.substr(last, i-last);
	l.push_back(new_str);
	last = i + 2;
	i += 2;
	ignore = false;
      }
    }
    else if(s[i] == delim && !ignore) {
      std::string new_str = s.substr(last, i - last);
      l.push_back(new_str);
      last = i+1;
    }

    ++i;
    
    if(times > 0 && l.size() >= times) {
      break;
    } 
  }
  if(s.size() - last > 0) {
    std::string new_str = s.substr(last, s.size() - last);
    l.push_back(new_str);
  }
}

static double toRad(double degrees) { return (pi * degrees) / 180.0; }
static double toDeg(double radiant) { return (radiant * 180.0) / pi; }

double distance(double lat1d, double lon1d, double lat2d, double lon2d) {
  if (lat1d == lat2d && lon1d == lon2d) return 0.0;

  double rad_lat1 = toRad(lat1d);
  double rad_lon1 = toRad(lon1d);
  double rad_lat2 = toRad(lat2d);
  double rad_lon2 = toRad(lon2d);
  
  double sin_lat1 = sin(rad_lat1);
  double sin_lat2 = sin(rad_lat2);
  double cos_lat1 = cos(rad_lat1);
  double cos_lat2 = cos(rad_lat2);
  double lon1_min_lon2 = rad_lon1-rad_lon2;
  
  return toDeg(acos(sin_lat1*sin_lat2 + cos_lat1*cos_lat2*cos(lon1_min_lon2))) * 60.0;
}
