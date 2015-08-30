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

int CoordInRect(float x, float y, float l, float t, float r, float b) {
  return ((x >= l) && (x < r) && (y < t) && (y >= b));
}
