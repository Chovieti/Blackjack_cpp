#include "GameModel.h"

// public
GameModel::GameModel(QObject *parent) : QObject(parent) {}

void GameModel::NewGame() {
  ClearHands();
  game_state_ = kWAITING;
  game_result_ = kINPROGRESS;
  emit GameStateChanged(game_state_);
}

void GameModel::DealCards() {
  if (game_state_ != kWAITING)
    return;

  ClearHands();

  deck_.Populate();
  deck_.Shuffle();

  // Раздаем две карты игроку и дилеру
  deck_.Deal(player_hand_);
  deck_.Deal(player_hand_);
  deck_.Deal(dealer_hand_);
  deck_.Deal(dealer_hand_);

  // Открываем карты игрока и одну карту дилера
  for (Card *card : player_hand_.GetCards()) {
    card->SetFaceUp(true);
  }

  if (!dealer_hand_.GetCards().empty()) {
    dealer_hand_.GetCards()[0]->SetFaceUp(true);
  }

  game_state_ = kPLAYERTURN;
  emit GameStateChanged(game_state_);
  emit CardsDealt();
}

void GameModel::PlayerHit() {
  if (game_state_ != kPLAYERTURN)
    return;

  deck_.Deal(player_hand_);
  player_hand_.GetCards().back()->SetFaceUp(true);
  emit CardAddedToPlayer();

  if (player_hand_.GetTotal() > 21) {
    game_state_ = kDEALERTURN;
    for (Card *card : dealer_hand_.GetCards()) {
      card->SetFaceUp(true);
    }
    game_result_ = DetermineWinner();
    game_state_ = kGAMEOVER;
    emit GameStateChanged(game_state_);
    emit GameCompleted(game_result_);
  }
}

void GameModel::PlayerStand() {
  if (game_state_ != kPLAYERTURN)
    return;

  game_state_ = kDEALERTURN;
  emit GameStateChanged(game_state_);

  DealerPlay();

  game_result_ = DetermineWinner();
  game_state_ = kGAMEOVER;
  emit GameStateChanged(game_state_);
  emit GameCompleted(game_result_);
}

void GameModel::ClearHands() {
  player_hand_.Clear();
  dealer_hand_.Clear();
}

// private
void GameModel::DealerPlay() {
  for (Card *card : dealer_hand_.GetCards()) {
    card->SetFaceUp(true);
  }
  while (dealer_hand_.GetTotal() < 17) {
    deck_.Deal(dealer_hand_);
    dealer_hand_.GetCards().back()->SetFaceUp(true);
    emit CardAddedToDealer();
  }
}

GameModel::GameResult GameModel::DetermineWinner() {
  int player_score = player_hand_.GetTotal();
  int dealer_score = dealer_hand_.GetTotal();

  if (player_score > 21)
    return GameResult::kDEALERWINS;
  if (dealer_score > 21)
    return GameResult::kPLAYERWINS;
  if (player_score > dealer_score)
    return GameResult::kPLAYERWINS;
  if (player_score < dealer_score)
    return GameResult::kDEALERWINS;
  return GameResult::kDRAW;
}