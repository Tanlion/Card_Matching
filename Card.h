#ifndef _CARD_HEADER_
#define _CARD_HEADER_

#include <iostream>
#include <deque>
#include <cassert>
#include <string>
#include <algorithm>
#include <random>
#include <iomanip>

#include "Log.h"

using namespace std;

typedef enum _Color {
  HEARTS,
  SPADES,
  DIAMONDS,
  CLUBS,
  COLOR_MAX
} COLOR;

typedef struct _Card {
  unsigned int  Point;
  COLOR         Color;
  string        Str;
} Card;

class CCard {
public:
  CCard();
  ~CCard();

  void Shuffle();
  void Print();
  void Print(CLog &Log);

  Card Pop();
  void Push(Card card);
  
  unsigned int GetCount();

private:
  deque<Card> Cards;
  unsigned int Count;
};

#endif