#include "Log.h"
#include <fstream>

CLog::CLog() {
  
}

CLog::~CLog() {

}
  
void CLog::PushLog(string str) {
  Log.push_back(str);
}

void CLog::Save() {
  ofstream fout("log.txt", ios::out);
  for (vector<string>::iterator it=Log.begin(); it!=Log.end(); it++) {
    fout << *it;
  }
  fout.close();
}