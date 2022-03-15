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

#version 460 core

layout(location=0) uniform mat3 camera;
layout(location=0) in mat3 worldTransform;
layout(location=3) in uint frameIn;
out mat3 transform;
out uint frame;

void main() {
  transform = camera * worldTransform;
  frame = frameIn;
}
