#ifndef MAZE_SRC_CONTROLLER_CONTROLLER_H_
#define MAZE_SRC_CONTROLLER_CONTROLLER_H_

#include <QObject>

#include "../Model/model.h"
#include "../View/view.h"

namespace awfl {
class Controller : public QObject {
  Q_OBJECT
 public:
  Controller(View* view, Model* model);
  ~Controller() = default;

 private:
  View* view_{nullptr};
  Model* model_{nullptr};

  void MazeInitConnections();
  void CaveInitConnections();
};
}  // namespace awfl

#endif  // MAZE_SRC_CONTROLLER_CONTROLLER_H_
