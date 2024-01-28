#include "ui_view.h"
#include "view.h"

namespace awfl {
void View::CaveInitConnections() {
  // connect(ui_->birthLimitSpinBox, &QSpinBox::valueChanged, this, [&](int value) { emit CaveSetBirthLimit(value); });
    connect(ui_->birthLimitSpinBox, &QSpinBox::valueChanged, this, &View::CaveSetBirthLimit);

  connect(ui_->deadLimitSpinBox, &QSpinBox::valueChanged, this, &View::CaveSetDeathLimit);
  
  // connect(ui_->deadLimitSpinBox, &QSpinBox::valueChanged, this, [&](int value) { emit CaveSetDeathLimit(value); });

  connect(ui_->chanceSpinBox, &QDoubleSpinBox::valueChanged, this, [&]() {
    emit CaveSetProbability(ui_->chanceSpinBox->value() / 100.0);
  });

  connect(ui_->nextStepButton, &QPushButton::clicked, this, [&]() { CaveNextUpdate(); });

  connect(ui_->autoButton, &QPushButton::clicked, this, [&]() { CaveAutoUpdate(); });

  connect(ui_->stopButton, &QPushButton::clicked, this, [&]() { CaveStopUpdate(); });
}

void View::CaveLoadFileEmit(const QString& path) {
  emit CaveLoadFile(path.toStdString());

  std::pair<size_t, size_t> size{};
  emit CaveGetSize(size);

  ui_->lengthSpinBoxCave->setValue(size.first);
  ui_->widthSpinBoxCave->setValue(size.second);
}

bool View::CaveNextUpdate() {
  bool state = false;

  emit CaveSetBirthLimit(ui_->birthLimitSpinBox->value());
  emit CaveSetDeathLimit(ui_->deadLimitSpinBox->value());
  emit CaveUpdateState(state);
  update();

  return state;
}

void View::CaveAutoUpdate() {
  if (timer_ == nullptr) {
    timer_ = new QTimer(this);

    connect(timer_, &QTimer::timeout, this, [&]() {
      if (CaveNextUpdate() == false) {
        CaveStopUpdate();
      }
    });

    timer_->setInterval(ui_->delaySpinBox->value());
    timer_->start();
  }
}

void View::CaveStopUpdate() {
  if (timer_ != nullptr) {
    timer_->stop();
    delete timer_;
    timer_ = nullptr;
  }
}

void View::CavePrint(QPainter& paint) {
  int n = 500 / caveSize_.first;
  int m = 500 / caveSize_.second;

  for (size_t i = 0; i < caveSize_.first; ++i) {
    for (size_t j = 0; j < caveSize_.second; ++j) {
      bool state = false;
      emit CaveGetCell(state, i, j);

      if (state == false) {
        paint.fillRect(offset_ + j * m, offset_ + i * n, m, n, Qt::white);
      } else {
        paint.fillRect(offset_ + j * m, offset_ + i * n, m, n, Qt::black);
      }
    }
  }
}
}  // namespace awfl
