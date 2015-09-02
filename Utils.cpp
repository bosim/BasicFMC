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


#include "XPLMDefs.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"

#include "Utils.h"

std::string GetPluginDir() {
  char buf[256];
  std::string PluginDir;

#if IBM
  std::string DirectoryName = "Resources\\Plugins\\BS-FMC\\";
#else
  std::string DirectoryName = "Resources/plugins/BS-FMC/";
#endif
  
  XPLMGetSystemPath(buf);
  PluginDir = std::string(buf) + DirectoryName;
  return PluginDir;
}

std::string GetAirwayFilename() {
  char buf[256];
  std::string AwyFile;

#if IBM
  std::string DirectoryName = "Resources\\default data\\earth_awy.dat";
#else
  std::string DirectoryName = "Resources/default data/earth_awy.dat";
#endif
  
  XPLMGetSystemPath(buf);
  AwyFile = std::string(buf) + DirectoryName;
  return AwyFile;
}

std::string GetProcedureFilename(std::string Airport, bool star) {
#if IBM
  std::string DirectoryName = "FD_FMC\\" + Airport + "\\";
#else
  std::string DirectoryName = "FD_FMC/" + Airport + "/";
#endif
  
  std::string PluginDir = GetPluginDir();
  std::string ProcedureFilename = PluginDir + DirectoryName;
  ProcedureFilename += star ? "STAR_data.csv" : "SID_data.csv";
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
