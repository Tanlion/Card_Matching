#include <semaphore.h>
#include <pthread.h>
#include <cassert>
#include <sstream>
#include <iostream>
#include <fcntl.h>

using namespace std;

#include "Player.h"
#include "Card.h"

#define PLAYER_COUNT 3
#define _GAME_ROUND_ 3

int main() {
  CLog          LogObj;
  CCard         CardObj;
  string        PlayerName[PLAYER_COUNT];
  string        DealerName;
  sem_t         *PlayerSem[PLAYER_COUNT];
  sem_t         *Dealer;
  ThreadParam   PlayerParam[PLAYER_COUNT];
  pthread_t     ThreadId[PLAYER_COUNT];
  ostringstream oss;
  bool          RoundFinish;
  
  DealerName = "SemDealer";
  for (size_t ii=0; ii<PLAYER_COUNT; ii++) {
    oss << "SemPlayer" << (ii + 1);
    PlayerName[ii] = oss.str();
    oss.str("");
  }
  
  ///
  /// Init semaphore object
  ///
  Dealer = sem_open (DealerName.c_str(), O_CREAT, 0644, 0);
  assert (Dealer != SEM_FAILED);
  for (size_t ii=0; ii<PLAYER_COUNT; ii++) {
    PlayerSem[ii] = sem_open (PlayerName[ii].c_str(), O_CREAT, 0644, 0);
    assert (PlayerSem[ii] != SEM_FAILED);
  }
  

  ///
  /// Init
  ///
  for (size_t ii=0; ii<PLAYER_COUNT; ii++) {
    PlayerParam[ii].No = (unsigned int)(ii + 1);

    PlayerParam[ii].SelfOne = PlayerSem[ii];
    PlayerParam[ii].NextOne = PlayerSem[(ii+1)%PLAYER_COUNT];
    PlayerParam[ii].Dealer  = Dealer;
    
    PlayerParam[ii].Log     = &LogObj;
    PlayerParam[ii].TheCard = &CardObj;
    
    pthread_create (
      &ThreadId[ii],
      NULL,
      pthread_player,
      &PlayerParam[ii]
      );
  }
  
  for (size_t ii=0; ii<_GAME_ROUND_; ii++) {
    oss << "\n\nDEALER: shuffle " << endl;
    CardObj.Shuffle();
    oss << "Start Round: " << ii + 1 << endl;
    LogObj.PushLog(oss.str());
    oss.str("");
    
    CardObj.Print(LogObj);
    
    RoundFinish = false;
    for (size_t jj=0; jj<PLAYER_COUNT; jj++) {
      PlayerParam[jj].IsFinish = &RoundFinish;
      PlayerParam[jj].Hand.push_back(CardObj.Pop());
    }
    sem_post (PlayerSem[ii]);
    sem_wait (Dealer);
    
    for (size_t jj=0; jj<PLAYER_COUNT; jj++) {
      oss << "Player " << PlayerParam[jj].No << ": round completed" << endl;
      LogObj.PushLog(oss.str());
      oss.str("");
    }
  }
  
  LogObj.Save();
  
  sem_unlink (DealerName.c_str());
  for (size_t ii=0; ii<PLAYER_COUNT; ii++) {
    sem_unlink (PlayerName[ii].c_str());
  }
  
  return 0;
}