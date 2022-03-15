#version 460 core

layout(location=0) uniform mat3 camera;
layout(location=0) in mat3 worldTransform;
layout(location=3) in uint frameIn;
out mat3 transform;
out uint frame;

void main() {
  transform = camera * worldTransform;
  //transform = worldTransform;
  frame = frameIn;
}
