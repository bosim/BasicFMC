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

#include "Pages.h"

Pages::Pages() {
  this->page = NULL;
}

void Pages::RegisterPage(std::string name, Page* page) {
  this->pages.insert(PageMap::value_type(name, page));
}

bool Pages::SwitchPage(std::string name) {
  PageMap::iterator iter;
  PageMap::value_type value;
    
  iter = this->pages.find(name);
  if(iter == pages.end()) {
    return false;
  }

  this->page = iter->second;
  return true;
}

Page* Pages::CurrentPage() {
  return this->page;
}

Pages::PageMapIterators Pages::PagesIterator() {
  return PageMapIterators(this->pages.begin(), this->pages.end());
}
