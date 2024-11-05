#ifndef PAGELAYOUT_HPP
#define PAGELAYOUT_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QPair>

static inline QPair<QHBoxLayout *, QLabel *> getPageLayout(const QString &name, const QString &value = "Loading...") {
    QHBoxLayout *cpu_name_layout = new QHBoxLayout;
    cpu_name_layout->addWidget(new QLabel(name));
    QLabel *value_label = new QLabel(value);
    cpu_name_layout->addWidget(value_label);

    return {cpu_name_layout, value_label};
}

#endif
