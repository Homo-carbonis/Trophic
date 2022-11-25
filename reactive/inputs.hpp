#include "reactive.hpp"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "../graphics.hpp"
#include <iostream> //debug
namespace reactive {
  auto mousePosition(const Window& window)
  {
    auto f = [&](float t) {
      int xi, yi;
      float x, y, w, h;
      SDL_GetMouseState(&xi,&yi);
      x = (float)xi;
      y = (float)yi;
      auto size = window.size();
      w = (float)size[0];
      h = (float)size[1];
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

  auto windowSize(const Window& window)
  {
    auto f = [&](float t){
      return window.size();
    };
    return makeSignal<glm::vec2>(f);
  }

  auto windowScale(const Window& window)
  {
    auto f = [&](float t){
      float w,h;
      auto size = window.size();
      w = (float)size[0];
      h = (float)size[1];
      return glm::vec2{1,w/h};
    };
    return scale(makeSignal<glm::vec2>(f));
  }
}
