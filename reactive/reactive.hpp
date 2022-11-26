#ifndef REACTIVE_H
#define REACTIVE_H
#include <functional>
#include <memory>
#include <cmath>

namespace reactive {
  template <typename T>
    class Signal {
      public:
        using type = T;
        T value;
        float time;
        T operator()(float t)
        {
          if (time != t) {
            time = t;
            value = f(t);
          }
          return value;
        }
        Signal(std::function<T(float)> f)
          :f(f) {}
      private:
        const std::function<T(float)> f;
    };

  template<typename T>
    using SignalPtr = std::shared_ptr<Signal<T>>;

  template<typename T>
    SignalPtr<T> makeSignal(std::function<T(float t)> f) 
    {
      return std::make_shared<Signal<T>>(f);
    }
   

  //Time Operators

  template<typename A, typename B> //B cast to float
    auto timeOffset(SignalPtr<A> s, B x)
    {
      return makeSignal<A>([=](float t){ return (*s)(t+x); } );
    }

  template<typename A>
    auto timeScale(SignalPtr<A> s, float x)
    {
      return makeSignal<A>([=](float t){ return (*s)(t*x); } );
    }


  
  //Control Operators
  template<typename A>
  auto either(SignalPtr<bool> p, SignalPtr<A> a, SignalPtr<A> b)
  {
    auto f = [=](float t)
    {
      return p ? (*a)(t):(*b)(t);
    };
    return makeSignal<A>(f);
  }

  auto risingEdge(SignalPtr<bool> a)
  {
    auto f = [=, value = std::make_shared<bool>(false)](float t)
    {
      return (*a)(t) && ! *value;
    };
    return makeSignal<bool>(f);
  }

  auto fallingEdge(SignalPtr<bool> a)
  {
    auto f = [=, value = std::make_shared<bool>(false)](float t)
    {
      return !(*a)(t) && *value;
    };
    return makeSignal<bool>(f);
  }

  auto edge(SignalPtr<bool> a)
  {
    auto f = [=, value = std::make_shared<bool>(false)](float t)
    {
      return (*a)(t) != *value;
    };
    return makeSignal<bool>(f);
  }

  template<typename A>
  auto latch(SignalPtr<A> a, SignalPtr<bool> gate)
  {
    auto f = [=, value = std::make_shared<A>()](float t) mutable
    {
      if ((*gate)(t))
        value = std::make_shared<A>((*a)(t));
      return *value;
    };
    return makeSignal<A>(f);
  }

  //Nullary Signals
  template<typename A>
  auto constant(A c)
  {
    return makeSignal<A>([=](float t){ return c; } );
  }

  auto linear()
  {
    return makeSignal<float>([](float t){ return t; } );
  }

  auto sin()
  {
    return makeSignal<float>([](float t){ return std::sin(t); } );
  }
  auto cos()
  {
    return makeSignal<float>([](float t){ return std::cos(t); } );
  }
  auto tan()
  {
    return makeSignal<float>([](float t){ return std::tan(t); } );
  }

  //Arithmetic Operators

  template<typename A>
  auto operator-(SignalPtr<A> a)
  {
    return makeSignal<A>([=](float t){ return -(*a)(t); } );
  }

  template<typename A, typename B>
  auto operator+(SignalPtr<A> a, SignalPtr<B> b)
  {
    return makeSignal<A>([=](float t){ return (*a)(t) + (*b)(t); } );
  }
  template<typename A, typename B>
  auto operator+(SignalPtr<A> a, B b)
  {
    return makeSignal<A>([=](float t){ return (*a)(t) + b; } );
  }
  template<typename A, typename B>
    auto operator+(A a, SignalPtr<B> b)
  {
    return makeSignal<A>([=](float t){ return a + (*b)(t); } );
  }

  template<typename A, typename B>
  auto operator-(SignalPtr<A> a, SignalPtr<B> b)
  {
    return makeSignal<A>([=](float t){ return (*a)(t) - (*b)(t); } );
  }
  template<typename A, typename B>
  auto operator-(SignalPtr<A> a, B b)
  {
    return makeSignal<A>([=](float t){ return (*a)(t) - b; } );
  }
  template<typename A, typename B>
  auto operator-(A a, SignalPtr<B> b)
  {
    return makeSignal<A>([=](float t){ return a - (*b)(t); } );
  }
 
  template<typename A, typename B>
  auto operator*(SignalPtr<A> a, SignalPtr<B> b)
  {
    return makeSignal<A>([=](float t){ return (*a)(t) * (*b)(t); } );
  }
  template<typename A, typename B>
  auto operator*(SignalPtr<A> a, B b)
  {
    return makeSignal<A>([=](float t){ return (*a)(t) * b; } );
  }
  template<typename A, typename B>
  auto operator*(A a, SignalPtr<B> b)
  {
    return makeSignal<A>([=](float t){ return a * (*b)(t); } );
  }
 
  template<typename A, typename B>
  auto operator/(SignalPtr<A> a, SignalPtr<B> b)
  {
    return makeSignal<A>([=](float t){ return (*a)(t) / (*b)(t); } );
  }
  template<typename A, typename B>
  auto operator/(SignalPtr<A> a, B b)
  {
    return makeSignal<A>([=](float t){ return (*a)(t) / b; } );
  }
  template<typename A, typename B>
  auto operator/(A a, SignalPtr<B> b)
  {
    return makeSignal<A>([=](float t){ return a / (*b)(t); } );
  }

  template<typename A>
  auto abs(SignalPtr<A> a)
  {
    return makeSignal<A>([=](float t){ return std::abs((*a)(t)); } );
  }

  template<typename A, typename B>
  auto max(SignalPtr<A> a, SignalPtr<B> b)
  {
    return makeSignal<A>([=](float t){ return std::max((*a)(t),(*b)(t)); } );
  }

  template<typename A, typename B>
  auto max(SignalPtr<A> a, B b)
  {
    return makeSignal<A>([=](float t){ return std::max((*a)(t),b); } );
  }

  template<typename A, typename B>
  auto max(A a, SignalPtr<B> b)
  {
    return makeSignal<A>([=](float t){ return std::max(a,(*b)(t)); } );
  }

  template<typename A, typename B>
  auto min(SignalPtr<A> a, SignalPtr<B> b)
  {
    return makeSignal<A>([=](float t){ return std::min((*a)(t),(*b)(t)); } );
  }

  template<typename A, typename B>
  auto min(SignalPtr<A> a, B b)
  {
    return makeSignal<A>([=](float t){ return std::min((*a)(t),b); } );
  }

  template<typename A, typename B>
  auto min(A a, SignalPtr<B> b)
  {
    return makeSignal<A>([=](float t){ return std::min(a,(*b)(t)); } );
  }

}
#endif
