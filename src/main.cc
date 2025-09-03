#include <QApplication>
#include "GameTable.h"
#include "CardWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GameTable table;
    table.resize(800, 600);
    table.setWindowTitle("Blackjack Game");
    table.show();
    return app.exec();
}