#ifndef CPUPAGE_H
#define CPUPAGE_H

#include <QWidget>
#include <QHBoxLayout>

class CpuPage final : public QWidget {
  Q_OBJECT

public:
  explicit CpuPage( QWidget *parent = nullptr );
  inline ~CpuPage() {
      delete l;
  }

private:
  QHBoxLayout *l;
};

#endif
