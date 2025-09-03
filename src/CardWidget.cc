#include "CardWidget.h"
#include <QPainter>

CardWidget::CardWidget(const Card& card, QWidget* parent) 
    : QWidget(parent), m_card(card) {
    setFixedSize(100, 145);
}

QSize CardWidget::sizeHint() const {
    return QSize(100, 145);
}

void CardWidget::setCard(const Card& card) {
    m_card = card;
    update();
}

Card CardWidget::card() const {
    return m_card;
}

void CardWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    if (m_card.GetFaceUp()) {
        drawCardFront(painter);
    } else {
        drawCardBack(painter);
    }
}

void CardWidget::drawCardFront(QPainter& painter) {
    painter.setPen(Qt::black);
    painter.setBrush(Qt::white);
    painter.drawRoundedRect(rect(), 10, 10);
    
    QRect topLeftRect(5, 5, 20, 20);
    QRect bottomRightRect(width() - 25, height() - 25, 20, 20);
    
    drawCornerSymbols(painter, topLeftRect);
    drawCornerSymbols(painter, bottomRightRect);
    
    QRect centerRect(width()/2 - 30, height()/2 - 30, 60, 60);
    drawCenterSymbols(painter, centerRect);
}

void CardWidget::drawCardBack(QPainter& painter) {
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, QColor(30, 60, 120));
    gradient.setColorAt(1, QColor(10, 30, 80));
    
    painter.setPen(Qt::darkBlue);
    painter.setBrush(gradient);
    painter.drawRoundedRect(rect(), 10, 10);
    
    painter.setPen(QPen(Qt::white, 1, Qt::DotLine));
    for (int i = 0; i < width(); i += 8) {
        painter.drawLine(i, 0, i, height());
    }
    for (int i = 0; i < height(); i += 8) {
        painter.drawLine(0, i, width(), i);
    }
    
    painter.setPen(QPen(Qt::darkBlue, 3));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect(), 10, 10);
}

void CardWidget::drawCornerSymbols(QPainter& painter, const QRect& rect) {
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.setPen(suitColor());
    painter.drawText(rect, Qt::AlignCenter, m_card.toString());
}

void CardWidget::drawCenterSymbols(QPainter& painter, const QRect& rect) {
    painter.setFont(QFont("Arial", 36, QFont::Bold));
    
    QString symbol;
    switch (m_card.GetSuit()) {
    case Card::Hearts: symbol = "♥"; break;
    case Card::Diamonds: symbol = "♦"; break;
    case Card::Clubs: symbol = "♣"; break;
    case Card::Spades: symbol = "♠"; break;
    default: symbol = "★"; break;
    }
    
    painter.setPen(suitColor());
    painter.drawText(rect, Qt::AlignCenter, symbol);
}

QColor CardWidget::suitColor() const {
    switch (m_card.GetSuit()) {
    case Card::Hearts:
    case Card::Diamonds:
        return Qt::red;
    case Card::Clubs:
    case Card::Spades:
        return Qt::black;
    default:
        return Qt::darkYellow;
    }
}