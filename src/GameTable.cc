#include "GameTable.h"
#include <QApplication>
#include <QThread>

// Конструктор класса GameTable
GameTable::GameTable(QWidget *parent) : QWidget(parent) {
    SetupUI();  // Настраиваем пользовательский интерфейс
    NewGame();
}

// Деструктор класса GameTable
GameTable::~GameTable() {
    ClearTable();
}

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

    // Затем нужно будет сделать подключение сигналов кнопок к соответствующим слотам
    connect(deal_button_, &QPushButton::clicked, this, &GameTable::DealCards);
    connect(hit_button_, &QPushButton::clicked, this, &GameTable::PlayerHit);
    connect(stand_button_, &QPushButton::clicked, this, &GameTable::PlayerStand);
    connect(new_game_button_, &QPushButton::clicked, this, &GameTable::NewGame);
}

void GameTable::DealerPlay() {
    CalculateScores();
    while(dealer_hand_.GetTotal() < 17) {
        deck_.Deal(dealer_hand_);
        dealer_hand_.GetCards().back()->SetFaceUp(true);
        UpdateCardWidgets();
        CalculateScores();
        QThread::msleep(500);
    }
}

void GameTable::CalculateScores() {
    player_score_label_->setText(QString("Игрок: %1").arg(player_hand_.GetTotal()));
    dealer_score_label_->setText(QString("Дилер: %1").arg(dealer_hand_.GetTotal()));
}

void GameTable::DetermineWinner() {
    int player_score = player_hand_.GetTotal();
    int dealer_score = dealer_hand_.GetTotal();

    if (player_score > 21) {
        game_status_label_->setText("Вы проиграли! Перебор.");
    } else if (dealer_score > 21) {
        game_status_label_->setText("Вы выиграли! У дилера перебор.");
    } else if (player_score > dealer_score) {
        game_status_label_->setText("Вы выиграли!");
    } else if (player_score < dealer_score) {
        game_status_label_->setText("Дилер выиграл!");
    } else {
        game_status_label_->setText("Ничья!");
    }
}

void GameTable::ClearTable() {
    player_hand_.Clear();
    dealer_hand_.Clear();

    for (CardWidget* widget : player_card_widgets_) {
        player_layout_->removeWidget(widget);
        delete widget;
    }
    player_card_widgets_.clear();

    for (CardWidget* widget : dealer_card_widgets_) {
        dealer_layout_->removeWidget(widget);
        delete widget;
    }
    dealer_card_widgets_.clear();

    player_score_label_->setText("Игрок: 0");
    dealer_score_label_->setText("Дилер: 0");
}

void GameTable::UpdateCardWidgets() {
    // Очищаем текущие виджеты
    for (CardWidget* widget : player_card_widgets_) {
        player_layout_->removeWidget(widget);
        delete widget;
    }
    player_card_widgets_.clear();

    for (CardWidget* widget : dealer_card_widgets_) {
        dealer_layout_->removeWidget(widget);
        delete widget;
    }
    dealer_card_widgets_.clear();

    // Создаем виджеты для карт игрока
    for (Card* card : player_hand_.GetCards()) {
        CardWidget* card_widget = new CardWidget(*card, this);
        player_layout_->addWidget(card_widget);
        player_card_widgets_.append(card_widget);
    }
    // Создаем виджеты для карт дилера
    for (Card* card : dealer_hand_.GetCards()) {
        CardWidget* card_widget = new CardWidget(*card, this);
        dealer_layout_->addWidget(card_widget);
        dealer_card_widgets_.append(card_widget);
    }
    // Обновляем компоновку
    player_layout_->update();
    dealer_layout_->update();
}

// private slots
void GameTable::DealCards() {
    ClearTable();
    deck_.Populate();
    deck_.Shuffle();

    // Раздаем две карты игроку и дилеру
    deck_.Deal(player_hand_);
    deck_.Deal(player_hand_);
    deck_.Deal(dealer_hand_);
    deck_.Deal(dealer_hand_);

    // Открываем карты игрока и одну карту дилера
    for (Card* card : player_hand_.GetCards()) {
        card->SetFaceUp(true);
    }
    
    if (!dealer_hand_.GetCards().empty()) {
        dealer_hand_.GetCards()[0]->SetFaceUp(true);
    }

    UpdateCardWidgets();
    CalculateScores();

    game_in_progress_ = true;
    deal_button_->setEnabled(false);
    hit_button_->setEnabled(true);
    stand_button_->setEnabled(true);

    if (player_hand_.GetTotal() == 21) {
        game_status_label_->setText("Блэкджек! Нажмите 'Остановиться' для продолжения.");
    } else {
        game_status_label_->setText("Ваш ход. Возьмите карту или остановитесь.");
    }
}

void GameTable::PlayerHit() {
    if (!game_in_progress_) return;

    deck_.Deal(player_hand_);
    player_hand_.GetCards().back()->SetFaceUp(true);

    UpdateCardWidgets();
    CalculateScores();

    if (player_hand_.GetTotal() > 21) {
        game_status_label_->setText("Перебор! Вы проиграли.");
        hit_button_->setEnabled(false);
        stand_button_->setEnabled(false);
        game_in_progress_ = false;

        for (Card* card : dealer_hand_.GetCards()) {
            card->SetFaceUp(true);
        }
        UpdateCardWidgets();
        CalculateScores();
    }
}

void GameTable::PlayerStand() {
    if (!game_in_progress_) return;

    hit_button_->setEnabled(false);
    stand_button_->setEnabled(false);

    for (Card* card : dealer_hand_.GetCards()) {
        card->SetFaceUp(true);
    }
    UpdateCardWidgets();

    DealerPlay();
    DetermineWinner();
    
    game_in_progress_ = false;
}

void GameTable::NewGame() {
    ClearTable();
    game_status_label_->setText("Нажмите 'Раздать' для начала игры");
    deal_button_->setEnabled(true);
    hit_button_->setEnabled(false);
    stand_button_->setEnabled(false);
}