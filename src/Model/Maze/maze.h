#ifndef MAZE_SRC_MODEL_MAZE_MAZE_H_
#define MAZE_SRC_MODEL_MAZE_MAZE_H_

#include <algorithm>
#include <fstream>
#include <random>
#include <string>
#include <vector>

namespace awfl {
struct Coord {
  Coord(size_t _x, size_t _y) : x(_x), y(_y) {}
  Coord(std::pair<size_t, size_t> pos) : x(pos.first), y(pos.second) {}
  size_t x{}, y{};
};

class Maze {
 public:
  Maze() = default;
  Maze(const std::string& path);
  Maze(size_t length, size_t width);
  ~Maze() = default;

  size_t GetLength() const noexcept;
  size_t GetWidth() const noexcept;

  std::pair<size_t, size_t> GetWalls(size_t i, size_t j) const noexcept;

  void SaveFile(const std::string& path) const;

  std::vector<Coord> SolveMaze(Coord begin, Coord end);

 private:
  std::vector<std::vector<bool>> rightWall_;
  std::vector<std::vector<bool>> bottomWall_;

  size_t length_{};
  size_t width_{};

  size_t counter_ = 1;

  std::vector<int> sideLine_;

  std::vector<std::vector<size_t>> solveMatrix_;

  void Generate();

  void AllocateMem();

  bool Random() const;

  bool Wave();
  std::vector<Coord> FindPath(Coord pos, bool hasPath) const;

  void FillEmptyValue();
  void AssignUniqueSet();
  void AddingVerticalWalls(size_t row);
  void MergeSet(size_t idx, int elem);
  void AddingHorizontalWalls(size_t row);
  int CalculateUniqueSet(int elem);
  void CheckedHorizontalWalls(size_t row);
  int CalculateHorizontalWalls(int elem, size_t row);
  void PreparatingNewLine(size_t row);
  void AddingEndLine();
  void CheckedEndLine();
};
}  // namespace awfl

#endif  // MAZE_SRC_MODEL_MAZE_MAZE_H_
