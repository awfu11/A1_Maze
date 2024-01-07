#ifndef MAZE_SRC_VIEW_VIEW_H_
#define MAZE_SRC_VIEW_VIEW_H_

#include <QCoreApplication>
#include <QFileDialog>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QString>
#include <QTimer>
#include <QWidget>

#include "../Model/model.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

namespace awfl {
class View : public QMainWindow {
  Q_OBJECT

 public:
  View(QWidget *parent = nullptr);
  ~View();

 signals:
  void MazeGenerate(size_t, size_t);
  void MazeLoadFile(const std::string &);
  void MazeSaveFile(const std::string &);
  void MazeGetSize(std::pair<size_t, size_t> &);
  void MazeGetWalls(std::pair<size_t, size_t> &, size_t, size_t);
  void MazeSolve(std::vector<Coord> &, std::pair<size_t, size_t>,
                 std::pair<size_t, size_t>);

  void CaveGenerate(size_t, size_t, double);
  void CaveSetBirthLimit(int);
  void CaveSetDeathLimit(int);
  void CaveSetProbability(double);
  void CaveLoadFile(const std::string &);
  void CaveSaveFile(const std::string &);
  void CaveUpdateState(bool &);
  void CaveGetCell(bool &, size_t, size_t);
  void CaveGetSize(std::pair<size_t, size_t> &);

 private:
  Ui::View *ui_;
  QTimer *timer_{nullptr};

  enum { MAZE, CAVE };

  int state_ = MAZE;
  bool init_ = false;

  std::pair<size_t, size_t> mazeSize_{};
  std::pair<size_t, size_t> caveSize_{};

  int offset_ = 10;

  QPoint firstPosition_;
  QPoint secondPosition_;

  std::pair<bool, bool> positionClicked_{};

  std::vector<Coord> path_;

  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

  void LoadFile();
  void SaveFile();
  void Generate();
  void InitConnections();

  QString GetDir();
  bool ValidPosition(const QPoint &pos);

  void MazeInitConnections();
  void MazeLoadFileEmit(const QString &path);
  void MazePrint(QPainter &paint);
  void MazePrintSelectedPosition(QPainter &paint);
  void MazePrintPath(QPainter &paint);

  void CaveInitConnections();
  void CaveAutoUpdate();
  bool CaveNextUpdate();
  void CaveStopUpdate();
  void CaveLoadFileEmit(const QString &path);
  void CavePrint(QPainter &paint);
};

}  // namespace s21
#endif  // MAZE_SRC_VIEW_VIEW_H_
