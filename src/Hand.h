#ifndef BLACKJACK_HAND_H
#define BLACKJACK_HAND_H

#include <vector>
#include "Card.h"

class Hand {
public:
    Hand();
    virtual ~Hand();
    void Add(Card *p_card);
    void Clear();
    int GetTotal() const;
    const std::vector<Card*>& GetCards() const { return cards_; }
protected:
    std::vector<Card*> cards_;
};

#endif  // BLACKJACK_HAND_H