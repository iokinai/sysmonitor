#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include "sidebarsetup.hpp"
#include "pages/cpupage.hpp"

#include <QDir>
#include <QIcon>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>

//"C:\\Users\\MegaD\\Downloads\\addicon.png"

MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent ), ui( new Ui::Sysmonitor ) {
  ui->setupUi( this );
  ui->sidebar->setAlignment( Qt::AlignTop );

  CpuPage *page = new CpuPage;
  QLabel *page1 = new QLabel;
  page1->setText("Page 1");
  QLabel *page2 = new QLabel;
  page2->setText("Page 2");
  QLabel *page3 = new QLabel;
  page3->setText("Page 3");
  QLabel *page4 = new QLabel;
  page4->setText("Page 4");

  ui->main->addWidget(page);
  ui->main->addWidget(page2);
  ui->main->addWidget(page3);
  ui->main->addWidget(page4);

  ui->sidebar->addWidget(getButton("C:\\Users\\MegaD\\Downloads\\addicon.png", page, ui->main));
  ui->sidebar->addWidget(getButton("C:\\Users\\MegaD\\Downloads\\addicon.png", page2, ui->main));
  ui->sidebar->addWidget(getButton("C:\\Users\\MegaD\\Downloads\\addicon.png", page3, ui->main));
  ui->sidebar->addWidget(getButton("C:\\Users\\MegaD\\Downloads\\addicon.png", page4, ui->main));

}

MainWindow::~MainWindow() {
  delete ui;
}
