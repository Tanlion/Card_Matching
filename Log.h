#ifndef _LOG_HEADER_
#define _LOG_HEADER_

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class CLog {
public:
  CLog();
  ~CLog();
  
  void PushLog(string str);
  void Save();
  
private:
  vector<string> Log;
};

#endif