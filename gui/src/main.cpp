#include "mainwindow.hpp"

#include <QApplication>

int main( int argc, char *argv[] ) {
    QCoreApplication::setAttribute(Qt::AA_NativeWindows, false);

  QApplication a( argc, argv );
  MainWindow w;
  w.show();
  return a.exec();
}
