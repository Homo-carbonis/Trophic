#version 460 core

layout(location=0) out vec4 colour;
layout(binding=0) uniform sampler2DArray tex;

in vec3 textureCoordinate;


void main() {
  colour = texture(tex, textureCoordinate);
  //colour = vec4(textureCoordinate,1.0);
  //colour = vec4(1.0,1.0,0.0,1.0);
}
