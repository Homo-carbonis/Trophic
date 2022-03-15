#include <fstream>
using namespace std;
char* readAll(istream &s)
{
  size_t length;
  s.seekg(0, ios::end);
  length = s.tellg();
  s.seekg(0, ios::beg);
  char* buffer = new char[length];
  s.read(buffer, length);
  buffer[length-1]='\0';
  return buffer;
}
