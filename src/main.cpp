#include <QApplication>

#include "Controller/controller.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  using namespace awfl;
  View view;
  Model model;
  Controller controller(&view, &model);

  return a.exec();
}
