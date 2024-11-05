#include "gpupage.hpp"
#include <QLabel>
#include <QWidget>
#include <osd_system_info.hpp>
#include <converters.hpp>
#include <gpu_info.hpp>
#include "pagelayout.hpp"
#include <QFuture>
#include <QtConcurrent>

GpuPage::GpuPage( QWidget *parent )
    : QWidget( parent ), globalLayout(new QVBoxLayout), info(nullptr) {
    globalLayout->setAlignment(Qt::AlignTop);
}

void GpuPage::updateGpuInfo(QLabel* gpunlabel, QLabel* gpudlabel, QLabel* gpudvlabel, QLabel* gpuvalabel,
                            QLabel* gpuvrtlabel, QLabel* gpurrlabel, QLabel* gpuhrlabel, QLabel* gpuvrlabel) {
    if (!info) return;

    gpunlabel->setText(QString::fromStdString(info->name));
    gpudlabel->setText(QString::fromStdString(info->description));
    gpudvlabel->setText(QString::fromStdString(info->driver_version));
    gpuvalabel->setText(QString::fromStdString(info->video_arch));
    gpuvrtlabel->setText(QString::fromStdString(info->vram_type));
    gpurrlabel->setText(QString::number(info->refresh_rate));
    gpuhrlabel->setText(QString::number(info->horizontal_resolution));
    gpuvrlabel->setText(QString::number(info->vertical_resolution));
}

void GpuPage::loadGpu() {
    if (globalLayout->count() != 0) return;

    auto [gpunlayout, gpunlabel] = getPageLayout("GPU");
    auto [gpudlayout, gpudlabel] = getPageLayout("Description");
    auto [gpudvlayout, gpudvlabel] = getPageLayout("Driver Version");
    auto [gpuvalayout, gpuvalabel] = getPageLayout("Video Arch");
    auto [gpuvrtlayout, gpuvrtlabel] = getPageLayout("VRAM Type");
    auto [gpurrlayout, gpurrlabel] = getPageLayout("Refresh Rate");
    auto [gpuhrlayout, gpuhrlabel] = getPageLayout("Horizontal Resolution");
    auto [gpuvrlayout, gpuvrlabel] = getPageLayout("Vertical Resolution");

    globalLayout->addLayout(gpunlayout);
    globalLayout->addLayout(gpudlayout);
    globalLayout->addLayout(gpudvlayout);
    globalLayout->addLayout(gpuvalayout);
    globalLayout->addLayout(gpuvrtlayout);
    globalLayout->addLayout(gpurrlayout);
    globalLayout->addLayout(gpuhrlayout);
    globalLayout->addLayout(gpuvrlayout);

    QFuture<sysmonitor::gpu_info> future = QtConcurrent::run([this] {
        return sysmonitor::load_gpu_info();
    });

    QFutureWatcher<sysmonitor::gpu_info> *watcher = new QFutureWatcher<sysmonitor::gpu_info>(this);
    connect(watcher, &QFutureWatcher<sysmonitor::gpu_info>::finished, this, [=, this]() {
        info = new sysmonitor::gpu_info(watcher->result());

        updateGpuInfo(gpunlabel, gpudlabel, gpudvlabel, gpuvalabel,
                      gpuvrtlabel, gpurrlabel, gpuhrlabel, gpuvrlabel);

        watcher->deleteLater();
    });

    watcher->setFuture(future);
    setLayout(globalLayout);
}
