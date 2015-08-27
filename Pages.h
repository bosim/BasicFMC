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

#ifndef PAGES_H
#define PAGES_H

#include <map>
#include <string>

#include "Page.h"


class Pages {
 public:
  typedef std::map<std::string, Page*> PageMap;
  typedef std::pair<PageMap::iterator, PageMap::iterator> PageMapIterators;

  Pages();

  void RegisterPage(std::string name, Page* page);
  bool SwitchPage(std::string name);
  Page* CurrentPage();
  PageMapIterators PagesIterator();

 private:
  PageMap pages;
  Page* page;

};

#endif
