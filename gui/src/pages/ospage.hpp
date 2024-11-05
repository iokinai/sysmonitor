#ifndef OSPAGE_H
#define OSPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <os_info.hpp>
#include <QLabel>

class OsPage : public QWidget
{
    Q_OBJECT
public:
    explicit OsPage( QWidget *parent = nullptr );
    inline ~OsPage() {
        delete globalLayout;
        delete info;
    }

    inline void showEvent(QShowEvent *event) override {
        QWidget::showEvent(event);
        loadOs();
    }

private:
    QVBoxLayout *globalLayout;
    sysmonitor::os_info *info;

    void loadOs();
    void updateOsInfo(QLabel* osnlabel, QLabel* ossnlabel, QLabel* osvlabel, QLabel* oscclabel,
                      QLabel* osloclabel, QLabel* ossdlabel, QLabel* oswdlabel, QLabel* osbdlabel);
};

#endif // OSPAGE_H
