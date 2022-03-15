// Copyright Ptolom 2022
// This file is part of Trophic.
//
// Trophic is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with this
// program. If not, see <https://www.gnu.org/licenses/>. 

#include <any>
#include <iostream> //temporary
class Raii {
  public:
    Raii(std::any o) : object_(o) {}
    Raii() {}
    Raii(const Raii&) = delete;
    Raii& operator=(const Raii&) = delete;

    Raii(Raii&& o) { object_.swap(o.object_); }

    Raii& operator=(Raii&& o)
    {
      if (object_.has_value()) {
        this->~Raii();
        object_.reset();
      }
      object_.swap(o.object_);
      return *this;
    }

    virtual ~Raii() {}
    bool hasObject() { return object_.has_value(); }
  protected:
    std::any object_;
};
