#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Sysmonitor;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow( QWidget *parent = nullptr );
  ~MainWindow();

private:
  Ui::Sysmonitor *ui;
};
#endif // MAINWINDOW_H
