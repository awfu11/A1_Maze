#include "view.h"

#include "ui_view.h"

namespace awfl {
View::View(QWidget *parent) : QMainWindow(parent), ui_(new Ui::View) {
  ui_->setupUi(this);
  ui_->tabWidget->setTabText(0, "Maze");
  ui_->tabWidget->setTabText(1, "Cave");

  this->setWindowTitle("Maze");

  InitConnections();
  MazeInitConnections();
  CaveInitConnections();
}

View::~View() { delete ui_; }

void View::InitConnections() {
  connect(ui_->tabWidget, &QTabWidget::currentChanged, this, [&]() {
    state_ = ui_->tabWidget->currentIndex();
    update();
  });

  connect(ui_->loadFileButton, &QPushButton::clicked, this, [&]() { LoadFile(); });

  connect(ui_->saveFileButton, &QPushButton::clicked, this, [&]() { SaveFile(); });

  connect(ui_->generateButton, &QPushButton::clicked, this, [&]() { Generate(); });
}

void View::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter paint(this);

  if (state_ == MAZE && init_) {
    emit MazeGetSize(mazeSize_);

    MazePrint(paint);
    MazePrintPath(paint);
    MazePrintSelectedPosition(paint);
  } else if (state_ == CAVE && init_) {
    emit CaveGetSize(caveSize_);

    CavePrint(paint);
  }

  paint.end();
}

void View::Generate() {
  init_ = true;
  if (state_ == MAZE) {
    ui_->clearPathButton->clicked();
    emit MazeGenerate(ui_->lengthSpinBoxMaze->value(), ui_->widthSpinBoxMaze->value());
  } else if (state_ == CAVE) {
    emit CaveGenerate(ui_->lengthSpinBoxCave->value(), ui_->widthSpinBoxCave->value(),
                      ui_->chanceSpinBox->value() / 100.0);
  }

  update();
}

QString View::GetDir() {
  QString dir = QCoreApplication::applicationDirPath();
  auto pos = dir.indexOf("/build", 0);
  return dir.left(pos);
}

void View::LoadFile() {
  QString path = QFileDialog::getOpenFileName(this, tr("Open file"), GetDir(), tr("files (*.txt)"));

  if (!path.isEmpty()) {
    init_ = true;
    if (state_ == MAZE) {
      ui_->clearPathButton->clicked();
      MazeLoadFileEmit(path);
    } else if (state_ == CAVE) {
      CaveLoadFileEmit(path);
    }
    update();
  }
}

void View::SaveFile() {
  QString path = QFileDialog::getSaveFileName(this, tr("Save file"), GetDir(), tr("files (*.txt)"));

  if (!path.isEmpty()) {
    if (state_ == MAZE) {
      emit MazeSaveFile(path.toStdString());
    } else if (state_ == CAVE) {
      emit CaveSaveFile(path.toStdString());
    }
  }
}
}  // namespace awfl
