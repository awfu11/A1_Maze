#ifndef MAZE_SRC_MODEL_CAVE_CAVE_H_
#define MAZE_SRC_MODEL_CAVE_CAVE_H_

#include <algorithm>
#include <fstream>
#include <random>
#include <string>
#include <vector>

namespace awfl {
class Cave {
 public:
  Cave() = default;
  Cave(size_t length, size_t width);
  Cave(size_t length, size_t width, double probability);
  Cave(const std::string& path);
  ~Cave() = default;

  void SetBirthLimit(size_t birthLimit);
  void SetDeadLimit(size_t deadLimit);
  void SetProbability(double probability);

  size_t GetBirthLimit() const noexcept;
  size_t GetDeadLimit() const noexcept;
  double GetProbability() const noexcept;

  size_t GetLength() const noexcept;
  size_t GetWidth() const noexcept;
  bool GetCell(size_t i, size_t j) const noexcept;
  std::vector<std::vector<bool>> GetCave() const noexcept;

  bool Update();

  void SaveFile(const std::string& path) const;

 private:
  std::vector<std::vector<bool>> cave_;

  size_t length_{};
  size_t width_{};

  enum { DEAD, LIVE };

  size_t deadLimit_{};
  size_t birthLimit_{};

  double probability_ = 0.5;

  void AllocateMem();

  bool Random() const;

  void Generate();

  void ReadFile(const std::string& path);
  size_t NumberOfLiveNeighbors(size_t i, size_t j) const;
  bool CompareState(const std::vector<std::vector<bool>>& prev,
                    const std::vector<std::vector<bool>>& next) const;
};
}  // namespace awfl

#endif  // MAZE_SRC_MODEL_CAVE_CAVE_H_
