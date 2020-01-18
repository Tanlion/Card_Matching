#include "Card.h"
#include <sstream>

string PointSeq[] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
string ColorSeq[] = {"Hearts","Spades","Diamonds","Clubs"};

CCard::CCard() {
  for (size_t ii=0; ii<sizeof(PointSeq)/sizeof(PointSeq[0]); ii++) {
    for (size_t jj=0; jj<sizeof(ColorSeq)/sizeof(ColorSeq[0]); jj++) {
      Card card;
      card.Point = (unsigned int)(ii + 1);
      card.Color = COLOR(jj);
      card.Str   = PointSeq[ii] + "(" + ColorSeq[jj] + ")";
      Cards.push_back(card);
    }
  }

  Count = 52;
}

CCard::~CCard() {
}

void CCard::Shuffle() {
  random_shuffle (Cards.begin(), Cards.end());
}

void CCard::Print() {
  unsigned int Count = 0;
  for (deque<Card>::iterator it=Cards.begin(); it!=Cards.end(); it++) {
    if (Count > 0 && Count%4 == 0) {
      cout<<endl;
    }
    cout<<setw(12)<<it->Str<<" ";
    Count++;
  }
  cout<<endl;
}

void CCard::Print(CLog &Log) {
  unsigned int Count = 0;
  ostringstream oss;
  for (deque<Card>::iterator it=Cards.begin(); it!=Cards.end(); it++) {
    if (Count > 0 && Count%4 == 0) {
      oss<<endl;
    }
    oss<<setw(12)<<it->Str<<" ";
    Count++;
  }
  oss<<endl;
  Log.PushLog(oss.str());
  oss.str("");
}

Card CCard::Pop() {
  Card card;
  card = Cards.front();
  Cards.pop_front();
  Count--;
  return card;
}

void CCard::Push(Card card) {
  Cards.push_back(card);
  Count++;
}

unsigned int CCard::GetCount() {
    return Count;
}