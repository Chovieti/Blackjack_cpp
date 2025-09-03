#include "Hand.h"

Hand::Hand() {
    cards_.reserve(7);
}

Hand::~Hand() {
    Clear();
}

void Hand::Add(Card* p_card) {
    cards_.push_back(p_card);
}

void Hand::Clear() {
    for (auto& card : cards_) {
        delete card;
        card = nullptr;
    }
    cards_.clear();
}

int Hand::GetTotal() const {
    if (cards_.empty()) return 0;
    
    int total = 0;
    bool contains_ace = false;
    for (const auto& card : cards_) {
        if (card->GetFaceUp()) {
            int value = card->GetValue();
            total += value;
            if (value == Card::Ace) contains_ace = true;
        }
    }
    if (contains_ace && total <= 11) {
        total += 10;
    }
    return total;
}