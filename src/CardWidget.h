#ifndef BLACKJACK_CARD_WIDGET_H
#define BLACKJACK_CARD_WIDGET_H

#include "Card.h"
#include <QWidget>

class CardWidget : public QWidget {
public:
  explicit CardWidget(const Card &card, QWidget *parent = nullptr);
  QSize sizeHint() const override;

  void setCard(const Card &card);
  Card card() const;

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  Card m_card;

  void drawCardFront(QPainter &painter);
  void drawCardBack(QPainter &painter);
  void drawCornerSymbols(QPainter &painter, const QRect &rect);
  void drawCenterSymbols(QPainter &painter, const QRect &rect);
  QColor suitColor() const;
};

#endif // BLACKJACK_CARD_WIDGET_H