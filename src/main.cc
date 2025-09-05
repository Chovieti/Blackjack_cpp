#include "GameModel.h"
#include "GameTable.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  GameModel model;
  GameTable table(&model);
  table.resize(800, 600);
  table.setWindowTitle("Blackjack Game");
  table.show();
  return app.exec();
}