#ifndef BLACKJACK_DECK_H
#define BLACKJACK_DECK_H

#include "Hand.h"

class Deck : public Hand {
public:
  Deck();
  virtual ~Deck();

  void Populate();
  void Shuffle();
  void Deal(Hand &hand);
};

#endif // BLACKJACK_DECK_H