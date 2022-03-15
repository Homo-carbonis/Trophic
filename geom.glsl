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

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;
in mat3 transform[];
in uint frame[];
out vec3 textureCoordinate;

const vec3 clipSquare[] = {{-1.0,1.0,1.0}, {-1.0,-1.0,1.0}, {1.0,1.0,1.0}, {1.0,-1.0,1.0}};
const vec2 textureSquare[] = {{0.0,1.0}, {0.0,0.0}, {1.0,1.0}, {1.0,0.0}};

void main()
{
  for (int i=0; i<4; i++) {
    gl_Position.xyw = transform[0] * clipSquare[i];
    gl_Position.z=0.0;
    textureCoordinate.xy=textureSquare[i];
    textureCoordinate.z=float(frame[0]);
    EmitVertex();
  }
}
