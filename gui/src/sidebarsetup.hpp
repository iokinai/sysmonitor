#ifndef SIDEBARSETUP_HPP
#define SIDEBARSETUP_HPP

#include <QWidget>
#include <QPushButton>
#include <QMainWindow>
#include <QStackedWidget>
#include <QFile>
#include <QLatin1String>

inline QString readFromFile(const QString &filepath) {
    QFile file(filepath);

    if (!file.open(QFile::ReadOnly)) {
        qWarning("Couldn't open button style file");
    }

    return QLatin1String(file.readAll());
}

inline QPushButton *getButton(const QString &iconPath, QWidget *page, QStackedWidget *sw) {
    QPushButton *btn = new QPushButton();

    btn->setStyleSheet(readFromFile(":/styles/styles/sidebar_button.qss"));
    btn->setIcon( QIcon( iconPath ) );
    btn->setFixedSize( QSize( 50, 50 ) );
    btn->setIconSize(QSize( 25, 25 ));

    QObject::connect(btn, &QPushButton::clicked, [=]() { sw->setCurrentWidget(page); });

    return btn;
}

#endif // SIDEBARSETUP_HPP
