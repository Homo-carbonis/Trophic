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
    //textureCoordinate.z=frame[0];
    textureCoordinate.z=float(frame[0]);
    EmitVertex();
  }
}
