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
#include "XPLMNavigation.h"
#include "XPLMUtilities.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstring>

#include <vector>

class NavAidNotFoundException : public std::logic_error {
 public:
  NavAidNotFoundException(const std::string& what_arg) :
   std::logic_error(what_arg) {

  }
};

class NavAidInfo {
 public:
  XPLMNavRef ref;
  int type;
  float lat;
  float lon;
  float height;
  int freq;
  float heading;
  std::string id; /* ~ 32 */
  std::string name /* ~ 256 */;

  /* internal variables */
  int fmc_forced_speed;
  int fmc_forced_altitude;
  bool fmc_star;
  bool fmc_sid;
  float fmc_distance;
  
  NavAidInfo() : ref(0), type(0), lat(0.0), lon(0.0), height(0.0), freq(0),
    heading(0), id(""), name(""), fmc_forced_speed(0), fmc_forced_altitude(0),
    fmc_star(false), fmc_sid(false) {
  }
  NavAidInfo(XPLMNavRef& ref, int& type, float& lat, float& lon,
             float& height, int& freq, float& heading, char id[],
             char name[]) :
  ref(ref), type(type), lat(lat), lon(lon), height(height),
    freq(freq), heading(heading), id(id), name(name), fmc_forced_speed(0),
    fmc_forced_altitude(0), fmc_star(false), fmc_sid(false) {
  }
};

class Navigation {
 public:
  static NavAidInfo FindAirport(std::string Airport) {
    XPLMNavRef ref;

    /* Temp variables */
    int type, freq;
    float lat, lon, height, heading;
    char id[32];
    char name[256];

    ref = XPLMFindNavAid(NULL, Airport.c_str(), NULL,
                         NULL, NULL, xplm_Nav_Airport);

    if(ref == XPLM_NAV_NOT_FOUND) {
      throw NavAidNotFoundException("Airport not found");
    }

    XPLMGetNavAidInfo(ref, &type, &lat, &lon, &height,
                      &freq, &heading, id, name, NULL);

    return NavAidInfo(ref, type, lat, lon, height, freq, heading, id, name);
  }

  static NavAidInfo FindNavAidIdLonLat(std::string NavAid, float lon, float lat) {
    /* Temp variables */
    int type, freq;
    float height, heading;
    char id[32];
    char name[256];
    XPLMNavRef ref;
    
    ref = XPLMFindNavAid(NULL, NavAid.c_str(), &lat, &lon, NULL,
                                    xplm_Nav_NDB | xplm_Nav_VOR);

    if(ref == XPLM_NAV_NOT_FOUND) {
      ref = XPLMFindNavAid(NULL, NavAid.c_str(), &lat, &lon, NULL,
                           xplm_Nav_Fix);      
    }

    if(ref == XPLM_NAV_NOT_FOUND) {
      throw NavAidNotFoundException("Navaid not found");
    }
    
    XPLMGetNavAidInfo(ref, &type, &lat, &lon, &height,
                      &freq, &heading, id, name, NULL);

    return NavAidInfo(ref, type, lat, lon, height, freq, heading, id, name);
  }
  
  static void FindNavAid(std::string NavAid, std::vector<NavAidInfo>& Result) {

    /* Temp variables */
    int type, freq;
    float lat, lon, height, heading;
    char id[32];
    char name[256];

    for(XPLMNavRef ref = XPLMGetFirstNavAid();
        ref != XPLM_NAV_NOT_FOUND;
        ref = XPLMGetNextNavAid(ref)) {

      XPLMGetNavAidInfo(ref, &type, &lat, &lon, &height,
                        &freq, &heading, id, name, NULL);

      if(!strcmp(id, NavAid.c_str())) {
        Result.push_back(NavAidInfo(ref, type, lat, lon, height, freq, heading, id, name));
      }
    }
  }
};
