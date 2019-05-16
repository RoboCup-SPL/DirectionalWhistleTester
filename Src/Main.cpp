/**
 * @file Main.cpp
 *
 * This file defines the main procedure of the program.
 *
 * @author Arne Hasselbring
 */

#include "MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  MainWindow window;
  window.show();

  return app.exec();
}
