#include "Card.h"

Card::Card(Suit suit, Rank rank) : suit_(suit), rank_(rank), face_up_(false) {}

Card::Suit Card::GetSuit() const { return suit_; }
Card::Rank Card::GetRank() const { return rank_; }

bool Card::GetFaceUp() const { return face_up_; }
void Card::SetFaceUp(bool faceUp) { face_up_ = faceUp; }

QString Card::toString() const {
  static const QString suits[] = {"♥", "♦", "♣", "♠"};
  static const QString ranks[] = {"",  "A", "2", "3",  "4", "5", "6",
                                  "7", "8", "9", "10", "J", "Q", "K"};

  return QString("%1%2").arg(ranks[rank_]).arg(suits[suit_]);
}