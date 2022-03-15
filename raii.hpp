#include <any>
#include <iostream> //temporary
class Raii {
  public:
    Raii(std::any o) : object_(o) {}
    Raii() {}
    Raii(const Raii&) = delete;
    Raii& operator=(const Raii&) = delete;

    Raii(Raii&& o) { object_.swap(o.object_); }

    Raii& operator=(Raii&& o)
    {
      if (object_.has_value()) {
        this->~Raii();
        object_.reset();
      }
      object_.swap(o.object_);
      return *this;
    }

    virtual ~Raii() {}
    bool hasObject() { return object_.has_value(); }
  protected:
    std::any object_;
};
