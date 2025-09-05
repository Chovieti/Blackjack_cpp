#include "Deck.h"
#include <chrono>
#include <random>

Deck::Deck() {
  cards_.reserve(52);
  Populate();
}

Deck::~Deck() {}

void Deck::Populate() {
  Clear();
  for (int s = Card::Hearts; s <= Card::Spades; ++s) {
    for (int r = Card::Ace; r <= Card::King; ++r) {
      Add(new Card(static_cast<Card::Suit>(s), static_cast<Card::Rank>(r)));
    }
  }
}

void Deck::Shuffle() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::shuffle(cards_.begin(), cards_.end(), rng);
}

void Deck::Deal(Hand &hand) {
  if (!cards_.empty()) {
    hand.Add(cards_.back());
    cards_.pop_back();
  }
}