#include "cpupage.hpp"
#include <QLabel>
#include <QWidget>
#include <osd_system_info.hpp>
#include <converters.hpp>
#include <cpu_info.hpp>
#include "pagelayout.hpp"
#include <QFuture>
#include <QtConcurrent>

CpuPage::CpuPage( QWidget *parent )
    : QWidget( parent ), globalLayout(new QVBoxLayout), info(nullptr) {
    globalLayout->setAlignment(Qt::AlignTop);
}

void CpuPage::updateCpuInfo(QLabel* cpunlabel, QLabel* cpudlabel, QLabel* cpualabel, QLabel* cpuslabel,
                            QLabel* cpuclabel, QLabel* cputlabel, QLabel* cpumlabel, QLabel* cpullabel,
                            QLabel* cpul2label, QLabel* cpul3label) {
    if (!info) return;

    cpunlabel->setText(QString::fromStdString(info->cpu_name));
    cpudlabel->setText(QString::fromStdString(info->description));
    cpualabel->setText(QString::fromStdString(info->arch));
    cpuslabel->setText(QString::fromStdString(info->socket));
    cpuclabel->setText(QString::number(info->cores));
    cputlabel->setText(QString::number(info->threads));
    cpumlabel->setText(QString::fromStdString(sysmonitor::hertz_converter(info->max_speed, 2)));
    cpullabel->setText(QString::fromStdString(sysmonitor::percent_converter(info->load_percentage)));
    cpul2label->setText(QString::fromStdString(sysmonitor::bytes_converter(info->l2_size, 2)));
    cpul3label->setText(QString::fromStdString(sysmonitor::bytes_converter(info->l3_size, 2)));
}

void CpuPage::loadCpu() {
    if (globalLayout->count() != 0) return;

    auto [cpunlayout, cpunlabel] = getPageLayout("CPU Name");
    auto [cpudlayout, cpudlabel] = getPageLayout("Description");
    auto [cpualayout, cpualabel] = getPageLayout("Architecture");
    auto [cpuslayout, cpuslabel] = getPageLayout("Socket");
    auto [cpuclayout, cpuclabel] = getPageLayout("Cores");
    auto [cputlayout, cputlabel] = getPageLayout("Threads");
    auto [cpumlayout, cpumlabel] = getPageLayout("Max Speed");
    auto [cpullayout, cpullabel] = getPageLayout("Load");
    auto [cpul2layout, cpul2label] = getPageLayout("L2 Size");
    auto [cpul3layout, cpul3label] = getPageLayout("L3 Size");

    globalLayout->addLayout(cpunlayout);
    globalLayout->addLayout(cpudlayout);
    globalLayout->addLayout(cpualayout);
    globalLayout->addLayout(cpuslayout);
    globalLayout->addLayout(cpuclayout);
    globalLayout->addLayout(cputlayout);
    globalLayout->addLayout(cpumlayout);
    globalLayout->addLayout(cpullayout);
    globalLayout->addLayout(cpul2layout);
    globalLayout->addLayout(cpul3layout);

    QFuture<sysmonitor::cpu_info> future = QtConcurrent::run([this] {
        return sysmonitor::load_cpu_info();
    });

    QFutureWatcher<sysmonitor::cpu_info> *watcher = new QFutureWatcher<sysmonitor::cpu_info>(this);
    connect(watcher, &QFutureWatcher<sysmonitor::cpu_info>::finished, this, [=, this]() {
        info = new sysmonitor::cpu_info(watcher->result());

        updateCpuInfo(cpunlabel, cpudlabel, cpualabel, cpuslabel, cpuclabel, cputlabel, cpumlabel, cpullabel, cpul2label, cpul3label);

        watcher->deleteLater();
    });

    watcher->setFuture(future);
    setLayout(globalLayout);
}
