#include "reactive.hpp"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "../graphics.hpp"
#include <iostream>
using namespace std;
namespace reactive {
  class Handler {
    Handler()
    : mouseWheel(0),
      mouseWheelSignal(makeSignal<float>( [&](float t){ cout<< mouseWheel << endl; return mouseWheel; } )),
      windowSizeSignal(makeSignal<glm::vec2>( [&](float t){ return windowSize; } )){}
    public:
      static Handler* getInstance()
      {
        if(instance == nullptr){
          instance = new Handler();
        }
        return instance;
      }
      bool handle() {
        while(SDL_PollEvent(&event)) {
          switch (event.type) {
            case SDL_MOUSEWHEEL: handleMouseWheel(event.wheel); break;
            case SDL_QUIT: return false;
            case SDL_WINDOWEVENT: handleWindow(event.window); break;
          }
        }
        return true;
      }

      SignalPtr<float> mouseWheelSignal;
      SignalPtr<glm::vec2> windowSizeSignal;

   
    private:
      static Handler* instance;
      void handleWindow(SDL_WindowEvent e)
      {
        switch (e.event) {
          case SDL_WINDOWEVENT_SIZE_CHANGED:
            float w = (float)e.data1;
            float h = (float)e.data2;
            glViewport(0, 0, w, h);
            windowSize={w,h};
        }
      }
      void handleMouseMotion(SDL_MouseMotionEvent e)
      {
        if (e.state & SDL_BUTTON_LMASK) {
        }
      }
      void handleMouseWheel(SDL_MouseWheelEvent e)
      {
        mouseWheel+=e.preciseY;
      }
      SDL_Event event;
      float mouseWheel;
      glm::vec2 windowSize;
  };

  Handler* Handler::instance = nullptr;


  auto mousePosition()
  {
    auto windowSize = Handler::getInstance()->windowSizeSignal;
    auto f = [=](float t) {
      int xi, yi;
      float x, y, w, h;
      SDL_GetMouseState(&xi,&yi);
      x = (float)xi;
      y = (float)yi;
      auto size = (*windowSize)(t);
      w=size[0];
      h=size[1];
      return glm::vec2{2*x/w -1 , -2*y/w + 1*h/w };
    };
    return makeSignal<glm::vec2>(f);
  }

  auto mouseButton(int button)
  {
    auto f = [=](float t) {
      uint32_t state;
      return SDL_GetMouseState(nullptr,nullptr) & SDL_BUTTON(button);
    };
    return makeSignal<bool>(f);
  }

  auto mouseWheel()
  {
    Handler* h = Handler::getInstance();
    return h->mouseWheelSignal;
  }

  auto windowSize()
  {
    Handler* h=Handler::getInstance();
    return h->windowSizeSignal;
  }

  auto windowScale()
  {
    auto windowSize = Handler::getInstance()->windowSizeSignal;
    auto f = [=](float t) {
      float w, h;
      auto size = (*windowSize)(t);
      w=size[0];
      h=size[1];
      return glm::vec2{1,w/h};
    };
    return scale(makeSignal<glm::vec2>(f));
  }
}
