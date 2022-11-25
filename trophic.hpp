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

#include <SDL2/SDL.h>
#include "graphics.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <functional>
#include "contiguous_map.hpp"
#include <png++/png.hpp>
#include <png++/solid_pixel_buffer.hpp>
#include <cstdint>
#include "uniform.hpp"
#include "reactive/reactive.hpp"
#include "reactive/transform.hpp"
#include "reactive/inputs.hpp"
using namespace std;

class Camera {
  public:
    Camera(reactive::SignalPtr<glm::mat3> signal)
      : uniform(0), signal(signal) { update(0); }
    void update(float t) { uniform.set((*signal)(t)); }
  private:
    reactive::SignalPtr<glm::mat3> signal;
    Uniform<glm::mat3> uniform;
};


using arrayType = VertexArray<attribute<glm::mat3,GL_FLOAT>, attribute<glm::uvec1, GL_INT>>;
using bufferType = arrayType::bufferType;
using SpriteVertex = SLTuple::Tuple<glm::mat3,glm::uvec1>;

class SpriteInstance {
  public: 
    SpriteInstance(int id, contiguous_map<int,SpriteVertex>& vertices)
      : id(id), vertices(vertices)
    {
      transform(matrix::identity);
      frame(0);
    }
    glm::mat3 transform() { return transform_(); }
    void transform(glm::mat3 t) { transform_() = t; }
    unsigned int frame() { return frame(); }
    void frame(unsigned int i) { frame_() = i; };
    unsigned int nextFrame(unsigned int) { return ++frame_(); };
  private:
    SpriteVertex& vertex() { return vertices[id]; }
    glm::mat3& transform_() { return SLTuple::get<0>(vertex()); }
    unsigned int& frame_() { return SLTuple::get<1>(vertex())[0]; }
    int id;
    contiguous_map<int,SpriteVertex>& vertices;
};

class Sprite {
  public:
    Sprite(bufferType& vbo, Texture& t)
      : vbo(vbo), texture(t), frameCount(t.layerCount()) {}
    SpriteInstance& newInstance()
    {
      return instances.emplace_back(nextID++, vertices);
    }
    void draw(float t) {
      vbo.bufferData(vertices.size(), vertices.data());
      texture.bind();
      glDrawArrays(GL_POINTS, 0, vertices.size());
    }
    int frameCount;
  private:
    bufferType& vbo;
    Texture& texture;
    int nextID=0;
    vector<SpriteInstance> instances;
    contiguous_map<int,SpriteVertex> vertices;
};

class EventHandler {
  public:
    EventHandler(Camera& camera) : camera(camera) {}
    bool handle() {
      while(SDL_PollEvent(&event)) {
        switch (event.type) {
          case SDL_AUDIODEVICEADDED: break;
          case SDL_AUDIODEVICEREMOVED: break;
          case SDL_CONTROLLERAXISMOTION: break;
          case SDL_CONTROLLERBUTTONDOWN: break;
          case SDL_CONTROLLERBUTTONUP: break;
          case SDL_CONTROLLERDEVICEADDED: break;
          case SDL_CONTROLLERDEVICEREMOVED: break;
          case SDL_CONTROLLERDEVICEREMAPPED: break;
          case SDL_DOLLARGESTURE: break;
          case SDL_DOLLARRECORD: break;
          case SDL_DROPFILE: break;
          case SDL_DROPTEXT: break;
          case SDL_DROPBEGIN: break;
          case SDL_DROPCOMPLETE: break;
          case SDL_FINGERMOTION: break;
          case SDL_FINGERDOWN: break;
          case SDL_FINGERUP: break;
          case SDL_KEYDOWN: break;
          case SDL_KEYUP: break;
          case SDL_JOYAXISMOTION: break;
          case SDL_JOYBALLMOTION: break;
          case SDL_JOYHATMOTION: break;
          case SDL_JOYBUTTONDOWN: break;
          case SDL_JOYBUTTONUP: break;
          case SDL_JOYDEVICEADDED: break;
          case SDL_JOYDEVICEREMOVED: break;
          case SDL_MOUSEMOTION: handleMouseMotion(event.motion); break;
          case SDL_MOUSEBUTTONDOWN: break;
          case SDL_MOUSEBUTTONUP: break;
          case SDL_MOUSEWHEEL: handleMouseWheel(event.wheel); break;
          case SDL_MULTIGESTURE: break;
          case SDL_QUIT: return false;
          case SDL_SYSWMEVENT: break;
          case SDL_TEXTEDITING: break;
          case SDL_TEXTINPUT: break;
          case SDL_USEREVENT: break;
          case SDL_WINDOWEVENT: handleWindow(event.window); break;
        }
      }
      return true;
    }
  private:
    void handleWindow(SDL_WindowEvent e)
    {
      switch (e.event) {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
          windowWidth = (float)e.data1;
          windowHeight = (float)e.data2;
          glViewport(0, 0, windowWidth, windowHeight);
      }
    }
    void handleMouseMotion(SDL_MouseMotionEvent e)
    {
      if (e.state & SDL_BUTTON_LMASK) {
      }
    }
    void handleMouseWheel(SDL_MouseWheelEvent e)
    {
    }
    SDL_Event event;
    Camera& camera;
    float windowWidth, windowHeight;
};


class World {
  public:
    World()
      : context("Trophic", {800,600}),
      program{loadShader<VertexShader>("shaders/vert.glsl"),
        loadShader<GeometryShader>("shaders/geom.glsl"),
        loadShader<FragmentShader>("shaders/frag.glsl")},
      vbo(vao.newBuffer(0,GL_STREAM_DRAW)),
      camera( reactive::windowScale(context.window) * reactive::translate(reactive::mousePosition(context.window)) * reactive::scale(reactive::constant((float)0.1))),
      handler(camera) {}
    Sprite& newSprite(Texture& texture)
    {
      sprites.emplace_back(vbo, texture);
      return sprites.back();
    }
    void draw(float t)
    {
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      for (Sprite& s : sprites) { s.draw(t); }
      context.window.swap();
    }
    void run() 
    {
      uint32_t t1, t2, dt = 0;
      while (handler.handle())
      {
        t2=SDL_GetTicks();
        dt=t2-t1;
        t1=t2;
        camera.update((float)t1);
        draw(t1);
      }
    }
    Context context;
  private:
    Program program;
    arrayType vao;
    bufferType vbo;
    SDL_Event event;
    vector<Sprite> sprites;
    Camera camera;
    EventHandler handler;
};
