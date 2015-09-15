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

#ifndef FLIGHT_H
#define FLIGHT_H

#include "Navigation.h"
#include "ProcedureReader.h"

#include "XPLMDataAccess.h"

#include <vector>

class Flight {

 public:
  NavAidInfo dep_airport;
  NavAidInfo dest_airport;
  std::string flightno;
  std::vector<NavAidInfo> flightplan;
  std::vector<Procedure> sids;
  std::vector<std::string> sids_runways;
  std::vector<Procedure> stars;
  std::vector<std::string> stars_runways;
  
  void SetDepAirport(std::string Airport) {
    this->dep_airport = Navigation::FindAirport(Airport);
    this->sids.clear();
    this->sids_runways.clear();
    this->procedure_reader.ReadSidFile(GetProcedureFilename(Airport), this->sids);
    this->procedure_reader.GetRunways(this->sids, this->sids_runways);
  }

  void SetDestAirport(std::string Airport) {
    this->dest_airport = Navigation::FindAirport(Airport);
    this->stars.clear();
    this->stars_runways.clear();
    this->procedure_reader.ReadStarFile(GetProcedureFilename(Airport, true), this->stars);
    this->procedure_reader.GetRunways(this->stars, this->stars_runways);
  }

  void SyncToXPFMC() {
    size_t i;

    for(i=0; i < this->flightplan.size(); i++) {
      if(this->flightplan[i].type >= 0) {
        XPLMSetFMSEntryInfo(i+1, this->flightplan[i].ref,
                            this->flightplan[i].ComputedAltitude());
      } else {
        XPLMSetFMSEntryLatLon(i+1, this->flightplan[i].lat,
                              this->flightplan[i].lon,
                              this->flightplan[i].ComputedAltitude());
      }
    }
    for(; i < 99; i++) {
      XPLMClearFMSEntry(i+1);
    }

    XPLMSetDestinationFMSEntry(1);

  }

  void CalcDistances() {
    XPLMDataRef lat_ref = XPLMFindDataRef("sim/flightmodel/position/latitude");
    XPLMDataRef lon_ref = XPLMFindDataRef("sim/flightmodel/position/longitude");

    float lat = XPLMGetDataf(lat_ref);
    float lon = XPLMGetDataf(lon_ref);

    for(size_t i=0; i < this->flightplan.size(); i++) {
      if(i==0) {
        this->flightplan[0].fmc_distance = distance(lat, lon, this->flightplan[0].lat, this->flightplan[0].lon);
      }
      else {
        this->flightplan[i].fmc_distance = this->flightplan[i-1].fmc_distance;
        this->flightplan[i].fmc_distance += distance(this->flightplan[i-1].lat,
                                                    this->flightplan[i-1].lon,
                                                    this->flightplan[i].lat,
                                                    this->flightplan[i].lon);
      }
    }
  }
 private:
  ProcedureReader procedure_reader;
};

#endif
