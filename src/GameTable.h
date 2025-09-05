#ifndef BLACKJACK_GAME_TABLE_H
#define BLACKJACK_GAME_TABLE_H

#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "CardWidget.h"
#include "GameModel.h"

class GameTable : public QWidget {
public:
  explicit GameTable(GameModel *model, QWidget *parent = nullptr);
  ~GameTable();

  void UpdateView();

public slots:
  // Слоты для нажатия кнопок
  void HandleDealButton(); // Слот для раздачи карт
  void HandleHitButton(); // Слот для действия "взять карту"
  void HandleStandButton(); // Слот для действия "остановиться"
  void HandleNewGameButton(); // Слот для начала новой игры

  // Слоты для обработки изменений в модели
  void OnGameStateChanged();
  void OnCardsDealt();
  void OnCardAddedToPlayer();
  void OnCardAddedToDealer();
  void OnGameCompleted(GameModel::GameResult result);

private:
  GameModel *model_ = nullptr;

  QList<CardWidget *> player_card_widgets_; // Виджеты карт игрока
  QList<CardWidget *> dealer_card_widgets_; // Виджеты карт дилера

  // Компоновка интерфейса
  QVBoxLayout *main_layout_ = nullptr; // Основной вертикальный layout
  QHBoxLayout *dealer_layout_ =
      nullptr; // Горизонтальный layout для карт дилера
  QHBoxLayout *player_layout_ =
      nullptr; // Горизонтальный layout для карт игрока
  QHBoxLayout *button_layout_ = nullptr; // Layout для кнопок управления

  // Кнопки управления
  QPushButton *deal_button_ = nullptr; // Кнопка "Раздать"
  QPushButton *hit_button_ = nullptr;  // Кнопка "Взять"
  QPushButton *stand_button_ = nullptr; // Кнопка "Остановиться"
  QPushButton *new_game_button_ = nullptr; // Кнопка "Новая игра"

  // Метки для отображения информации
  QLabel *player_score_label_ = nullptr; // Метка счета игрока
  QLabel *dealer_score_label_ = nullptr; // Метка счета дилера
  QLabel *game_status_label_ = nullptr;  // Метка статуса игры

  void SetupUI(); // Настройка пользовательского интерфейса
  void ConnectModelSignals();
  // Вспомогательные методы для обновления UI
  void UpdateCardWidgets(); // Обновление виджетов карт
  void ClearCardWidgets();
  void UpdateScores();
  void UpdateGameStatus();
  void UpdateButtonStates();
};

#endif // BLACKJACK_GAME_TABLE_H