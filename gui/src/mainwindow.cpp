#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include "sidebarsetup.hpp"
#include "pages/gpupage.hpp"
#include "pages/cpupage.hpp"
#include "pages/ospage.hpp"
#include "pages/rampage.hpp"

#include <QDir>
#include <QIcon>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>
#include <QButtonGroup>

MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent ), ui( new Ui::Sysmonitor ) {
  ui->setupUi( this );
  ui->sidebar->setAlignment( Qt::AlignTop );

  centralWidget()->setStyleSheet(readFromFile(":/styles/styles/window.qss"));
  ui->main->setStyleSheet(readFromFile(":/styles/styles/pages.qss"));

  CpuPage *cpupage = new CpuPage;
  GpuPage *gpupage = new GpuPage;
  OsPage *ospage = new OsPage;
  RamPage *rampage = new RamPage;

  ui->main->addWidget(cpupage);
  ui->main->addWidget(gpupage);
  ui->main->addWidget(ospage);
  ui->main->addWidget(rampage);

  QButtonGroup *sidebargroup = new QButtonGroup(this);
  sidebargroup->setExclusive(true);

  QPushButton *cpubtn = getButton(":/icons/icons/cpu_white.png", cpupage, ui->main);
  cpubtn->setCheckable(true);
  QPushButton *gpubtn = getButton(":/icons/icons/gpu_white.png", gpupage, ui->main);
  gpubtn->setCheckable(true);
  QPushButton *osbtn = getButton(":/icons/icons/os_white.png", ospage, ui->main);
  osbtn->setCheckable(true);
  QPushButton *rambtn = getButton(":/icons/icons/memory_white.png", rampage, ui->main);
  rambtn->setCheckable(true);

  sidebargroup->addButton(cpubtn);
  sidebargroup->addButton(gpubtn);
  sidebargroup->addButton(osbtn);
  sidebargroup->addButton(rambtn);

  ui->sidebar->addWidget(cpubtn);
  ui->sidebar->addWidget(gpubtn);
  ui->sidebar->addWidget(osbtn);
  ui->sidebar->addWidget(rambtn);

  ui->main->setCurrentWidget(cpupage);
  cpubtn->setChecked(true);
}

MainWindow::~MainWindow() {
  delete ui;
}
