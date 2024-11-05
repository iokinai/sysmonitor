#ifndef CPUPAGE_H
#define CPUPAGE_H

#include <QWidget>
#include <QHBoxLayout>
#include <cpu_info.hpp>
#include <QLabel>

class CpuPage final : public QWidget {
  Q_OBJECT

public:
  explicit CpuPage( QWidget *parent = nullptr );
  inline ~CpuPage() {
      delete globalLayout;
      delete info;
  }

  inline void showEvent(QShowEvent *event) override {
      QWidget::showEvent(event);
      loadCpu();
  }

private:
  QVBoxLayout *globalLayout;
  sysmonitor::cpu_info *info;

  void loadCpu();
  void updateCpuInfo(QLabel* cpunlabel, QLabel* cpudlabel, QLabel* cpualabel, QLabel* cpuslabel,
                     QLabel* cpuclabel, QLabel* cputlabel, QLabel* cpumlabel, QLabel* cpullabel,
                     QLabel* cpul2label, QLabel* cpul3label);
};

#endif
