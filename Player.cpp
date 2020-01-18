#include "Player.h"
#include <unistd.h>
#include <cstdlib>
#include <sstream>

void *pthread_player(void *argv) {
  ThreadParam	          *param;
  deque<Card>::iterator ItWalker;
  deque<Card>::iterator FirstCard;
  bool			        IsMatched;
  bool                  AmIWin;
  ostringstream         oss;
  unsigned int          Round;
  
  param = (ThreadParam *)argv;
  AmIWin = false;
  Round = 1;
  
  while (1) {
    sem_wait (param->SelfOne);

    ///
    ///	Return the cards that were handed
    ///
    if (*param->IsFinish) {
      //HAND card1 card2
      //WIN yes (or no)
      //DECK contents of the deck, separated by spaces (e.g., 1 2 3)
      cout << "Player " << param->No << endl;
      cout << "HAND";
      
      for (ItWalker=param->Hand.begin(); ItWalker!=param->Hand.end(); ItWalker++) {
        cout << " " << ItWalker->Str;
        param->TheCard->Push(*ItWalker);
      }
      cout << endl;
      if (AmIWin) {
        cout << "WIN yes" << endl;
        param->TheCard->Print ();
      } else {
        cout << "WIN no" << endl;
      }
      cout << endl;
      AmIWin = false;
      Round++;
      param->Hand.clear();
      assert (param->Hand.size() == 0);
      
      if (param->TheCard->GetCount() == 52) {
        sem_post (param->Dealer);
      } else {
        sem_post (param->NextOne);
      }
    } else {
      ///
      ///	Hand a card
      ///
      oss << "Player " << param->No << ": hand     " << param->Hand[0].Str << endl;
      param->Hand.push_back(param->TheCard->Pop());
      oss << "Player " << param->No << ": draws    " << param->Hand[1].Str << endl;
      param->Log->PushLog(oss.str());
      oss.str("");
    
      ///
      /// Try to verify whether I am WIN
      ///
      for ( FirstCard=param->Hand.begin(), ItWalker=param->Hand.begin(), IsMatched = true;
            ItWalker != param->Hand.end();
            ItWalker++
          ) {
        if (FirstCard->Point != ItWalker->Point) {
          IsMatched = false;
          break;
        }
      }
    
      if (IsMatched) {
        ///
        /// Win, trigger dealer.
        ///
        oss << "Player " << param->No << ": hand     " << param->Hand[0].Str << " " << param->Hand[1].Str <<endl;
        oss << "Player " << param->No << ": wins" << endl;
        param->Log->PushLog(oss.str());
        oss.str("");
        AmIWin = true;
        *param->IsFinish = true;
        cout << "Round: " << Round << endl;
      } else {
        ///
        /// Discard a card by randomly.
        ///
        ItWalker = param->Hand.begin() + (rand () % param->Hand.size ());
        oss << "Player " << param->No << ": discards " << ItWalker->Str << endl;
        param->TheCard->Push (*ItWalker);
        param->Hand.erase (ItWalker);
        oss << "Player " << param->No << ": hand     " << param->Hand[0].Str << endl;
        param->Log->PushLog(oss.str());
        oss.str("");
      }
      
      ///
      /// Trigger next player to start
      ///
      sem_post (param->NextOne);
    }
  }
}
