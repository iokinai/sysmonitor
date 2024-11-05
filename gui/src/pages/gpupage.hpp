#ifndef GPUPAGE_HPP
#define GPUPAGE_HPP
#include <QWidget>
#include <QHBoxLayout>
#include <gpu_info.hpp>
#include <QLabel>

class GpuPage final : public QWidget {
    Q_OBJECT

public:
    explicit GpuPage( QWidget *parent = nullptr );
    inline ~GpuPage() {
        delete globalLayout;
        delete info;
    }

    inline void showEvent(QShowEvent *event) override {
        QWidget::showEvent(event);
        loadGpu();
    }

private:
    QVBoxLayout *globalLayout;
    sysmonitor::gpu_info *info;

    void loadGpu();
    void updateGpuInfo(QLabel* gpunlabel, QLabel* gpudlabel, QLabel* gpudvlabel, QLabel* gpuvalabel,
                       QLabel* gpuvrtlabel, QLabel* gpurrlabel, QLabel* gpuhrlabel, QLabel* gpuvrlabel);
};

#endif // GPUPAGE_HPP
