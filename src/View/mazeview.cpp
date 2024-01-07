#include "ui_view.h"
#include "view.h"

namespace awfl {
void View::MazeInitConnections() {
  connect(ui_->clearPathButton, &QPushButton::clicked, this, [&]() {
    positionClicked_ = {false, false};
    path_.clear();
    update();
  });
}

void View::MazeLoadFileEmit(const QString& path) {
  emit MazeLoadFile(path.toStdString());

  std::pair<size_t, size_t> size{};
  emit MazeGetSize(size);
  ui_->lengthSpinBoxMaze->setValue(size.first);
  ui_->widthSpinBoxMaze->setValue(size.second);
}

bool View::ValidPosition(const QPoint& pos) {
  return ((pos.x() >= offset_ && pos.x() <= 500 + offset_) &&
          (pos.y() >= offset_ && pos.y() <= 500 + offset_));
}

void View::mousePressEvent(QMouseEvent* event) {
  QPoint pos = event->pos();

  if (ui_->tabWidget->currentIndex() != MAZE || ValidPosition(pos) == false) {
    return;
  }

  pos.setX(floor((pos.x() - offset_) / (500 / mazeSize_.first)));
  pos.setY(floor((pos.y() - offset_) / (500 / mazeSize_.second)));

  if (event->button() == Qt::LeftButton) {
    positionClicked_.first = true;
    if (pos != secondPosition_ || !positionClicked_.second) {
      firstPosition_ = pos;
    } else {
      positionClicked_.first = false;
      positionClicked_.second = false;
    }
  } else if (event->button() == Qt::RightButton) {
    positionClicked_.second = true;
    if (pos != firstPosition_ || !positionClicked_.first) {
      secondPosition_ = pos;
    } else {
      positionClicked_.first = false;
      positionClicked_.second = false;
    }
  }
}

void View::MazePrintPath(QPainter& paint) {
  if (positionClicked_.first && positionClicked_.second) {
    path_.clear();
    emit MazeSolve(path_, {firstPosition_.y(), firstPosition_.x()},
                   {secondPosition_.y(), secondPosition_.x()});
  } else {
    return;
  }

  if (!path_.empty()) {
    int n = 500 / mazeSize_.first;
    int m = 500 / mazeSize_.second;

    paint.setPen(QPen(Qt::blue, 2));

    std::pair<double, double> from, to;
    from = {offset_ + path_.front().x * n + n / 2,
            offset_ + path_.front().y * m + m / 2};

    for (auto iter = ++path_.begin(); iter != path_.end(); ++iter) {
      to = {offset_ + iter->x * n + n / 2, offset_ + iter->y * m + m / 2};

      paint.drawLine(QPoint(from.second, from.first), QPoint(to.second, to.first));

      from = to;
    }
  }
}

void View::MazePrintSelectedPosition(QPainter& paint) {
  int n = 500 / mazeSize_.first;
  int m = 500 / mazeSize_.second;

  QPoint firstPos = {offset_ + firstPosition_.x() * n + 2 * n / 10,
                     offset_ + firstPosition_.y() * m + 2 * m / 10};
  QPoint secondPos = {offset_ + secondPosition_.x() * n + 2 * n / 10,
                      offset_ + secondPosition_.y() * m + 2 * m / 10};

  if (positionClicked_.first) {
    paint.setPen(QPen(Qt::white, 1));
    paint.setBrush(Qt::green);
    paint.drawRect(firstPos.x(), firstPos.y(), n - (4 * n / 10),
                   m - (4 * m / 10));
  }

  if (positionClicked_.second) {
    paint.setPen(QPen(Qt::white, 1));
    paint.setBrush(Qt::red);
    paint.drawRect(secondPos.x(), secondPos.y(), n - (4 * n / 10),
                   m - (4 * m / 10));
  }
  update();
}

void View::MazePrint(QPainter& paint) {
  int n = 500 / mazeSize_.first;
  int m = 500 / mazeSize_.second;

  paint.setPen(QPen(Qt::white, 2));
  paint.setBrush(Qt::black);

  if (init_) {
    paint.drawRect(offset_, offset_, n * mazeSize_.first, m * mazeSize_.second);
  }

  for (size_t i = 0; i < mazeSize_.first; ++i) {
    for (size_t j = 0; j < mazeSize_.second; ++j) {
      std::pair<size_t, size_t> walls;
      emit MazeGetWalls(walls, i, j);

      if (walls.first) {
        paint.drawLine(
            QPoint((j + 1) * m, i * n) + QPoint(offset_, offset_),
            QPoint((j + 1) * m, i * n + n) + QPoint(offset_, offset_));
      }

      if (walls.second) {
        paint.drawLine(
            QPoint(j * m, (i + 1) * n) + QPoint(offset_, offset_),
            QPoint(j * m + m, (i + 1) * n) + QPoint(offset_, offset_));
      }
    }
  }
}
}  // namespace awfl
