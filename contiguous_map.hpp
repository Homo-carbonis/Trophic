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
