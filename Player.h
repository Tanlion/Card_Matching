#ifndef _PLAYER_HEADER_
#define _PLAYER_HEADER_

#include <semaphore.h>
#include <unistd.h>
#include <vector>

#include "Card.h"
#include "Log.h"

using namespace std;

typedef struct _thread_param {
  int No;

  sem_t *SelfOne;
  sem_t *NextOne;
  sem_t *Dealer;

  bool *IsFinish;

  CLog  *Log;
  CCard *TheCard;
  deque<Card> Hand;
} ThreadParam;

void *pthread_player(void *argv);

#endif