#include "ospage.hpp"
#include <QFuture>
#include <QtConcurrent>
#include <osd_system_info.hpp>
#include "pagelayout.hpp"

OsPage::OsPage( QWidget *parent )
    : QWidget( parent ), globalLayout(new QVBoxLayout), info(nullptr) {
    globalLayout->setAlignment(Qt::AlignTop);
}

void OsPage::updateOsInfo(QLabel* osnlabel, QLabel* ossnlabel, QLabel* osvlabel, QLabel* oscclabel,
                            QLabel* osloclabel, QLabel* ossdlabel, QLabel* oswdlabel, QLabel* osbdlabel) {
    if (!info) return;

    osnlabel->setText(QString::fromStdString(info->name));
    ossnlabel->setText(QString::fromStdString(info->sys_name));
    osvlabel->setText(QString::fromStdString(info->version));
    oscclabel->setText(QString::fromStdString(info->country_code));
    osloclabel->setText(QString::fromStdString(info->locale));
    ossdlabel->setText(QString::fromStdString(info->system_drive));
    oswdlabel->setText(QString::fromStdString(info->windows_directory));
    osbdlabel->setText(QString::fromStdString(info->boot_device));
}

void OsPage::loadOs() {
    if (globalLayout->count() != 0) return;

    auto [osnlayout, osnlabel] = getPageLayout("OS Name");
    auto [ossnlayout, ossnlabel] = getPageLayout("System Name");
    auto [osvlayout, osvlabel] = getPageLayout("Version");
    auto [oscclayout, oscclabel] = getPageLayout("Country Code");
    auto [osloclayout, osloclabel] = getPageLayout("Locale");
    auto [ossdlayout, ossdlabel] = getPageLayout("System Drive");
    auto [oswdlayout, oswdlabel] = getPageLayout("Windows Directory");
    auto [osbdlayout, osbdlabel] = getPageLayout("Boot Device");

    globalLayout->addLayout(osnlayout);
    globalLayout->addLayout(ossnlayout);
    globalLayout->addLayout(osvlayout);
    globalLayout->addLayout(oscclayout);
    globalLayout->addLayout(osloclayout);
    globalLayout->addLayout(ossdlayout);
    globalLayout->addLayout(oswdlayout);
    globalLayout->addLayout(osbdlayout);

    QFuture<sysmonitor::os_info> future = QtConcurrent::run([this] {
        return sysmonitor::load_os_info();
    });

    QFutureWatcher<sysmonitor::os_info> *watcher = new QFutureWatcher<sysmonitor::os_info>(this);
    connect(watcher, &QFutureWatcher<sysmonitor::os_info>::finished, this, [=, this]() {
        info = new sysmonitor::os_info(watcher->result());

        updateOsInfo(osnlabel, ossnlabel, osvlabel, oscclabel,
                      osloclabel, ossdlabel, oswdlabel, osbdlabel);

        watcher->deleteLater();
    });

    watcher->setFuture(future);
    setLayout(globalLayout);
}
