#ifndef SIDEBARSETUP_HPP
#define SIDEBARSETUP_HPP

#include <QWidget>
#include <QPushButton>
#include <QMainWindow>
#include <QStackedWidget>

inline QPushButton *getButton(const QString &iconPath, QWidget *page, QStackedWidget *sw) {
    QPushButton *btn = new QPushButton();

    btn->setIcon( QIcon( iconPath ) );
    btn->setFixedSize( QSize( 50, 50 ) );

    QObject::connect(btn, &QPushButton::clicked, [=]() { sw->setCurrentWidget(page); });

    return btn;
}

#endif // SIDEBARSETUP_HPP
