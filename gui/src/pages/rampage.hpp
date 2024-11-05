#ifndef RAMPAGE_HPP
#define RAMPAGE_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <ram_stick_info.hpp>
#include <QLabel>

class RamPage : public QWidget
{
    Q_OBJECT
public:
    explicit RamPage( QWidget *parent = nullptr );
    inline ~RamPage() {
        delete globalLayout;
    }

    inline void showEvent(QShowEvent *event) override {
        QWidget::showEvent(event);
        loadRam();
    }

private:
    QVBoxLayout *globalLayout;
    QVector<sysmonitor::ram_stick_info> info;

    void loadRam();
};

#endif // RAMPAGE_HPP
