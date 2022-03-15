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

#include <fstream>
using namespace std;
char* readAll(istream &s)
{
  size_t length;
  s.seekg(0, ios::end);
  length = s.tellg();
  s.seekg(0, ios::beg);
  char* buffer = new char[length];
  s.read(buffer, length);
  buffer[length-1]='\0';
  return buffer;
}
