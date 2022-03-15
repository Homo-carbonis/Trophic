#include <vector>
#include <iterator>
#include <unordered_map>
using namespace std;
template <typename K, typename T>
class contiguous_map {
  public:
    typedef K key_type;
    typedef T mapped_type;
    typedef pair<const K, T> value_type;
    typedef typename vector<T>::size_type size_type;
    typedef typename vector<T>::iterator iterator;
    size_type count(K k) { return m.count(k); }
   
    T& operator[](const K& k)
    {
      if(count(k))
        return v[m[k]];
      else {
        size_type i=v.size();
        m[k]=i;
        v.resize(i+1);
        return v[i];
      }
    }
    T& at(const K& k) { return v.at(m.at(k)); }
    pair<iterator,bool> insert(const value_type& val)
    {
      const K k = val.first;
      const T t = val.second;
      if(count(k))
        return make_pair(v.begin()+m[k],false);
      else {
        m.insert(make_pair(k,v.size()));
        iterator i = v.end();
        v.push_back(t);
        return make_pair(i,true);
      }
    }
    iterator erase(const K& k)
    {
      size_type i = m[k];
      m.erase(k);
      iterator next = v.erase(v.begin()+i);
      for (pair<K,size_type> p : m)
        if(p.second > i)
          --m[p.first];
      return next;
    }
    size_type size() {return v.size();}
    T* data() { return v.data(); }
    void clear()
    {
      v.clear();
      m.clear();
    }
  private:
    vector<T> v;
    unordered_map<K,size_type> m;
};
