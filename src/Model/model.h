#ifndef MAZE_SRC_MODEL_MODEL_H_
#define MAZE_SRC_MODEL_MODEL_H_

#include <QObject>

#include "Cave/cave.h"
#include "Maze/maze.h"

namespace awfl {
class Model : public QObject {
  Q_OBJECT
 public:
  Model() = default;
  ~Model();

 public slots:
  std::pair<size_t, size_t> MazeGetWalls(size_t i, size_t j) const noexcept;
  std::pair<size_t, size_t> MazeGetSize() const noexcept;

  void MazeLoadFile(const std::string& path);
  void MazeSaveFile(const std::string& path) const;
  void MazeGenerate(size_t length, size_t width);
  std::vector<Coord> MazeSolve(std::pair<size_t, size_t> from,
                               std::pair<size_t, size_t> to);

  void CaveSetProbability(double probability);
  void CaveSetBirthLimit(int birthLimit);
  void CaveSetDeathLimit(int deadLimit);
  bool CaveGetCell(size_t i, size_t j) const noexcept;
  std::pair<size_t, size_t> CaveGetSize() const noexcept;

  void CaveLoadFile(const std::string& path);
  void CaveSaveFile(const std::string& path) const;
  void CaveGenerate(size_t length, size_t width, double probability);
  bool CaveUpdate();

 private:
  Maze* maze_{nullptr};
  Cave* cave_{nullptr};
};
}  // namespace awfl

#endif  // MAZE_SRC_MODEL_MODEL_H_
