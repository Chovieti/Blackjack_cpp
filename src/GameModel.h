#ifndef BLACKJACK_GAME_MODEL_H
#define BLACKJACK_GAME_MODEL_H

#include "Card.h"
#include "Deck.h"
#include "Hand.h"

#include <QObject> // временно пока не появится контроллер

class GameModel : public QObject {
  Q_OBJECT
public:
  enum GameResult { kPLAYERWINS, kDEALERWINS, kDRAW, kINPROGRESS };
  enum GameState { kWAITING, kPLAYERTURN, kDEALERTURN, kGAMEOVER };

  GameModel(QObject *parent = nullptr);
  virtual ~GameModel() = default;
  // Основные игровые методы
  void NewGame();
  void DealCards();
  void PlayerHit();
  void PlayerStand();

  const Hand &GetPlayerHand() const { return player_hand_; }
  const Hand &GetDealerHand() const { return dealer_hand_; }
  int GetPlayerScore() const { return player_hand_.GetTotal(); }
  int GetDealerScore() const { return dealer_hand_.GetTotal(); }
  GameState GetGameState() const { return game_state_; }
  GameResult GetGameResult() const { return game_result_; }

  void ClearHands(); // Очистка рук игрока и дилера
signals:
  //  Сигналы об уведомлении изменения в модели
  void GameStateChanged(GameState new_state);
  void CardsDealt();
  void CardAddedToPlayer();
  void CardAddedToDealer();
  void GameCompleted(GameResult result);

protected:
private:
  Deck deck_;
  Hand player_hand_, dealer_hand_;
  GameState game_state_ = kWAITING;
  GameResult game_result_ = kINPROGRESS;

  void DealerPlay(); // Логика хода дилера
  GameResult DetermineWinner();
};

#endif // BLACKJACK_GAME_MODEL_H