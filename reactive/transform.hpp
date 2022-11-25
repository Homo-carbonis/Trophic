#include "reactive.hpp"
#include "matrixTransform.hpp"

namespace reactive {
  auto translate(SignalPtr<glm::vec2> tr) 
  {
    return makeSignal<glm::mat3>(
        [=](float t){ return matrix::translate((*tr)(t)); } );
  }

  auto translate(glm::vec2 tr) 
  {
    return translate(constant(tr));
  }


  auto rotate(SignalPtr<float> r) 
  {
    return makeSignal<glm::mat3>(
        [=](float t){ return matrix::rotate((*r)(t)); } );
  }

  auto rotate(float r) 
  {
    return rotate(constant(r));
  }


  auto scale(SignalPtr<glm::vec2> s) 
  {
    return makeSignal<glm::mat3>(
        [=](float t){ return matrix::scale((*s)(t)); } );
  }

  auto scale(glm::vec2 s) 
  {
    return scale(constant(s));
  }

  auto scale(SignalPtr<float> s) 
  {
    return makeSignal<glm::mat3>(
        [=](float t){ return matrix::scale((*s)(t)); } );
  }

  auto scale(float s) 
  {
    return scale(constant(s));
  }


  auto zip(SignalPtr<float> a, SignalPtr<float> b) 
  {
    return makeSignal<glm::vec2>(
        [=](float t){ return glm::vec2{ (*a)(t), (*b)(t) }; } );
  }
}
