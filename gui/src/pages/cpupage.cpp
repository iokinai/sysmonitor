#include "cpupage.hpp"
#include <QLabel>
#include <QWidget>
#include <osd_system_info.hpp>
#include <cpu_info.hpp>

CpuPage::CpuPage( QWidget *parent )
    : QWidget( parent ), l(new QHBoxLayout) {
    l->setAlignment(Qt::AlignTop);

    sysmonitor::cpu_info info = sysmonitor::load_cpu_info();

    QHBoxLayout *cpu_name_layout = new QHBoxLayout;
    cpu_name_layout->addWidget(new QLabel("CPU Name:"));
    cpu_name_layout->addWidget(new QLabel(QString(info.cpu_name.c_str())));

    l->addLayout(cpu_name_layout);
    setLayout(l);
}
