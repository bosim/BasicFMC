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


#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

#define pi 3.14159265358979323846

/* Filenames */
std::string GetPluginDir();
std::string GetAirwayFilename();
std::string GetFreeNavProcedureFilename(std::string Airport, bool star=false);
std::string GetGNSProcedureFilename(std::string Airport);

/* Misc functions */
int CoordInRect(float x, float y, float l, float t, float r, float b);
void SplitLine(std::string s, std::vector<std::string>& l, char delim, size_t times = 0);
double distance(double lat1d, double lon1d, double lat2d, double lon2d);

#endif
