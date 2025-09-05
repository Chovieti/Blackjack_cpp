#include "GameTable.h"
#include <QApplication>
#include <QThread>

// Конструктор класса GameTable
GameTable::GameTable(GameModel *model, QWidget *parent)
    : model_(model), QWidget(parent) {
  SetupUI(); // Настраиваем пользовательский интерфейс
  ConnectModelSignals();
  UpdateView();
}

// Деструктор класса GameTable
GameTable::~GameTable() { ClearCardWidgets(); }

// Настройка пользовательского интерфейса
void GameTable::SetupUI() {
  // Создаем основной вертикальный layout
  main_layout_ = new QVBoxLayout(this);

  // Создаем область для карт дилера
  dealer_layout_ = new QHBoxLayout();
  dealer_score_label_ = new QLabel("Дилер: 0");
  main_layout_->addWidget(dealer_score_label_);
  main_layout_->addLayout(dealer_layout_);

  // Создаем область для карт игрока
  player_layout_ = new QHBoxLayout();
  player_score_label_ = new QLabel("Игрок: 0");
  main_layout_->addWidget(player_score_label_);
  main_layout_->addLayout(player_layout_);

  // Создаем кнопки управления
  button_layout_ = new QHBoxLayout();
  deal_button_ = new QPushButton("Раздать");
  hit_button_ = new QPushButton("Взять");
  stand_button_ = new QPushButton("Остановиться");
  new_game_button_ = new QPushButton("Новая игра");

  // Изначально кнопки "Взять" и "Остановиться" отключены
  hit_button_->setEnabled(false);
  stand_button_->setEnabled(false);

  // Добавляем кнопки в layout
  button_layout_->addWidget(deal_button_);
  button_layout_->addWidget(hit_button_);
  button_layout_->addWidget(stand_button_);
  button_layout_->addWidget(new_game_button_);
  main_layout_->addLayout(button_layout_);

  // Создаем метку для отображения статуса игры
  game_status_label_ = new QLabel("Нажмите 'Раздать' для начала игры");
  main_layout_->addWidget(game_status_label_);

  // Затем нужно будет сделать подключение сигналов кнопок к соответствующим
  // слотам
  connect(deal_button_, &QPushButton::clicked, this,
          &GameTable::HandleDealButton);
  connect(hit_button_, &QPushButton::clicked, this,
          &GameTable::HandleHitButton);
  connect(stand_button_, &QPushButton::clicked, this,
          &GameTable::HandleStandButton);
  connect(new_game_button_, &QPushButton::clicked, this,
          &GameTable::HandleNewGameButton);
}

void GameTable::ConnectModelSignals() {
  // Подключаем сигналы модели к слотам представления
  connect(model_, &GameModel::GameStateChanged, this,
          &GameTable::OnGameStateChanged);
  connect(model_, &GameModel::CardsDealt, this, &GameTable::OnCardsDealt);
  connect(model_, &GameModel::CardAddedToPlayer, this,
          &GameTable::OnCardAddedToPlayer);
  connect(model_, &GameModel::CardAddedToDealer, this,
          &GameTable::OnCardAddedToDealer);
  connect(model_, &GameModel::GameCompleted, this, &GameTable::OnGameCompleted);
}

void GameTable::UpdateView() {
  UpdateCardWidgets();
  UpdateScores();
  UpdateGameStatus();
  UpdateButtonStates();
  update();
}

// public slots
// Слоты для обработки нажатий кнопок
void GameTable::HandleDealButton() { model_->DealCards(); }

void GameTable::HandleHitButton() { model_->PlayerHit(); }

void GameTable::HandleStandButton() { model_->PlayerStand(); }

void GameTable::HandleNewGameButton() { model_->NewGame(); }

// Слоты для обработки изменений в модели
void GameTable::OnGameStateChanged() { UpdateView(); }

void GameTable::OnCardsDealt() { UpdateView(); }

void GameTable::OnCardAddedToPlayer() { UpdateView(); }

void GameTable::OnCardAddedToDealer() { UpdateView(); }

void GameTable::OnGameCompleted(GameModel::GameResult result) {
  UpdateView();

  QString result_text;
  switch (result) {
  case GameModel::kPLAYERWINS:
    result_text = "Вы выиграли!";
    break;
  case GameModel::kDEALERWINS:
    result_text = "Вы проиграли!";
    break;
  case GameModel::kDRAW:
    result_text = "Ничья!";
    break;
  default:
    result_text = "Игра завершена";
  }
  game_status_label_->setText(result_text);
}

// Вспомогательные методы для обновления UI
void GameTable::UpdateCardWidgets() {
  ClearCardWidgets();
  // Создаем виджеты для карт игрока
  for (Card *card : model_->GetPlayerHand().GetCards()) {
    CardWidget *card_widget = new CardWidget(*card, this);
    player_layout_->addWidget(card_widget);
    player_card_widgets_.append(card_widget);
  }
  // Создаем виджеты для карт дилера
  for (Card *card : model_->GetDealerHand().GetCards()) {
    CardWidget *card_widget = new CardWidget(*card, this);
    dealer_layout_->addWidget(card_widget);
    dealer_card_widgets_.append(card_widget);
  }

  // Обновляем компоновку
  player_layout_->update();
  dealer_layout_->update();
}

void GameTable::ClearCardWidgets() {
  // Очищаем виджеты карт игрока
  for (CardWidget *widget : player_card_widgets_) {
    player_layout_->removeWidget(widget);
    delete widget;
  }
  player_card_widgets_.clear();

  // Очищаем виджеты карт дилера
  for (CardWidget *widget : dealer_card_widgets_) {
    dealer_layout_->removeWidget(widget);
    delete widget;
  }
  dealer_card_widgets_.clear();
}

void GameTable::UpdateScores() {
  player_score_label_->setText(
      QString("Игрок: %1").arg(model_->GetPlayerScore()));
  dealer_score_label_->setText(
      QString("Дилер: %1").arg(model_->GetDealerScore()));
}

void GameTable::UpdateGameStatus() {
  // Обновляем статус в зависимости от состояния модели
  switch (model_->GetGameState()) {
  case GameModel::kWAITING:
    game_status_label_->setText("Нажмите 'Раздать' для начала игры");
    break;
  case GameModel::kPLAYERTURN:
    game_status_label_->setText("Ваш ход. Возьмите карту или остановитесь.");
    break;
  case GameModel::kDEALERTURN:
    game_status_label_->setText("Ход дилера");
    break;
  case GameModel::kGAMEOVER:
  default:
    break;
  }
}

void GameTable::UpdateButtonStates() {
  bool is_waiting = (model_->GetGameState() == GameModel::kWAITING);
  bool is_player_turn = (model_->GetGameState() == GameModel::kPLAYERTURN);
  bool is_game_over = (model_->GetGameState() == GameModel::kGAMEOVER);

  deal_button_->setEnabled(is_waiting);
  hit_button_->setEnabled(is_player_turn);
  stand_button_->setEnabled(is_player_turn);
  new_game_button_->setEnabled(is_game_over);
}