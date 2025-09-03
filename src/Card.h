#ifndef BLACKJACK_CARD_H
#define BLACKJACK_CARD_H
#include <QString>

class Card {
public:
    enum Suit { Hearts, Diamonds, Clubs, Spades };
    enum Rank { 
        Ace = 1, Two, Three, Four, Five, Six, Seven, 
        Eight, Nine, Ten, Jack, Queen, King 
    };

    Card(Suit suit, Rank rank);
    
    Suit GetSuit() const;
    Rank GetRank() const;
    bool GetFaceUp() const;
    void SetFaceUp(bool faceUp);
    int GetValue() const { 
        int value = 0;
        if (GetFaceUp()) {
            value = static_cast<int>(rank_);
            if (value > 10) value = 10;
        }
        return value;
    }
    
    QString toString() const;
    
private:
    Suit suit_;
    Rank rank_;
    bool face_up_;
};

#endif  // BLACKJACK_CARD_H