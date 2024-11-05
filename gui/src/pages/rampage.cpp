#include "rampage.hpp"
#include <QFuture>
#include <QtConcurrent>
#include <QListWidget>
#include <osd_system_info.hpp>
#include <converters.hpp>
#include "pagelayout.hpp"

RamPage::RamPage( QWidget *parent )
    : QWidget( parent ), globalLayout(new QVBoxLayout) {
    globalLayout->setAlignment(Qt::AlignTop);
}

void RamPage::loadRam() {
    if (globalLayout->count() != 0) return;

    auto [ramlayout, ramlabel] = getPageLayout("RAM info");
    globalLayout->addLayout(ramlayout);

    QFuture<QVector<sysmonitor::ram_stick_info>> future = QtConcurrent::run([this] {
        std::vector<sysmonitor::ram_stick_info> loaded = sysmonitor::load_ram_info();
        return QVector<sysmonitor::ram_stick_info>(loaded.begin(), loaded.end());
    });

    QFutureWatcher<QVector<sysmonitor::ram_stick_info>> *watcher = new QFutureWatcher<QVector<sysmonitor::ram_stick_info>>(this);
    connect(watcher, &QFutureWatcher<QVector<sysmonitor::ram_stick_info>>::finished, this, [=, this]() {
        info = watcher->result();

        ramlabel->setText("");

        for (size_t i = 0; i < info.size(); ++i) {
            auto [ramsticklayout, ramsticklabel] = getPageLayout("Stick");
            ramsticklabel->setText(QString::number(i));
            ramsticklabel->setMargin(20);

            auto [ramsticknlayout, ramsticknlabel] = getPageLayout("Stick Name");
            auto [ramstickmlayout, ramstickmlabel] = getPageLayout("Model");
            auto [ramstickvlayout, ramstickvlabel] = getPageLayout("Version");
            auto [ramstickclayout, ramstickclabel] = getPageLayout("Capacity");
            auto [ramstickslayout, ramstickslabel] = getPageLayout("Speed");
            auto [ramstickdlayout, ramstickdlabel] = getPageLayout("Description");
            auto [ramstickmtlayout, ramstickmtlabel] = getPageLayout("Memory Type");

            globalLayout->addLayout(ramsticklayout);
            globalLayout->addLayout(ramsticknlayout);
            globalLayout->addLayout(ramstickmlayout);
            globalLayout->addLayout(ramstickvlayout);
            globalLayout->addLayout(ramstickclayout);
            globalLayout->addLayout(ramstickslayout);
            globalLayout->addLayout(ramstickdlayout);
            globalLayout->addLayout(ramstickmtlayout);

            ramsticknlabel->setText(QString::fromStdString(info[i].name));
            ramstickmlabel->setText(QString::fromStdString(info[i].model));
            ramstickvlabel->setText(QString::fromStdString(info[i].version));
            ramstickclabel->setText(QString::fromStdString(sysmonitor::ram_converter_into_gb(info[i].capacity)));
            ramstickslabel->setText(QString::fromStdString(sysmonitor::hertz_converter(info[i].speed, 2)));
            ramstickdlabel->setText(QString::fromStdString(info[i].description));
            ramstickmtlabel->setText(QString::fromStdString(info[i].memory_type));
        }

        watcher->deleteLater();
    });

    watcher->setFuture(future);
    setLayout(globalLayout);
}
