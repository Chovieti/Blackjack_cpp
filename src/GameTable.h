#ifndef BLACKJACK_GAME_TABLE_H
#define BLACKJACK_GAME_TABLE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QList>

#include "CardWidget.h"
#include "Deck.h"
#include "Hand.h"

class GameTable : public QWidget {
public:
    explicit GameTable(QWidget *parent = nullptr);
    ~GameTable();

private slots:
    void DealCards();   // Слот для раздачи карт
    void PlayerHit();   // Слот для действия "взять карту"
    void PlayerStand(); // Слот для действия "остановиться"
    void NewGame();     // Слот для начала новой игры

private:
    Deck deck_;
    Hand player_hand_;
    Hand dealer_hand_;

    QList<CardWidget*> player_card_widgets_;    // Виджеты карт игрока
    QList<CardWidget*> dealer_card_widgets_;    // Виджеты карт дилера

    // Компоновка интерфейса
    QVBoxLayout *main_layout_ = nullptr;  // Основной вертикальный layout
    QHBoxLayout *dealer_layout_ = nullptr;    // Горизонтальный layout для карт дилера
    QHBoxLayout *player_layout_ = nullptr;    // Горизонтальный layout для карт игрока
    QHBoxLayout *button_layout_ = nullptr;    // Layout для кнопок управления

    // Кнопки управления
    QPushButton *deal_button_ = nullptr;  // Кнопка "Раздать"
    QPushButton *hit_button_ = nullptr;   // Кнопка "Взять"
    QPushButton *stand_button_ = nullptr; // Кнопка "Остановиться"
    QPushButton *new_game_button_ = nullptr;  // Кнопка "Новая игра"

    // Метки для отображения информации
    QLabel *player_score_label_ = nullptr;    // Метка счета игрока
    QLabel *dealer_score_label_ = nullptr;    // Метка счета дилера
    QLabel *game_status_label_ = nullptr;     // Метка статуса игры

    bool game_in_progress_ = false; // Флаг, указывающий, идет ли игра

    void SetupUI(); // Настройка пользовательского интерфейса
    void DealerPlay();  // Логика хода дилера
    void CalculateScores(); // Подсчет очков
    void DetermineWinner(); // Определение победителя
    void ClearTable();  // Очистка игрового стола
    void UpdateCardWidgets(); // Обновление виджетов карт
};

#endif  // BLACKJACK_GAME_TABLE_H