#include "cave.h"

namespace awfl {
Cave::Cave(size_t length, size_t width) : length_(length), width_(width) {
  AllocateMem();
  Generate();
}

Cave::Cave(size_t length, size_t width, double probability)
    : length_(length), width_(width), probability_(probability) {
  AllocateMem();
  Generate();
}

Cave::Cave(const std::string& path) { ReadFile(path); }

void Cave::AllocateMem() { cave_.resize(length_, std::vector<bool>(width_)); }

void Cave::SetBirthLimit(size_t birthLimit) { birthLimit_ = birthLimit; }

void Cave::SetDeadLimit(size_t deadLimit) { deadLimit_ = deadLimit; }

void Cave::SetProbability(double probability) { probability_ = probability; }

size_t Cave::GetBirthLimit() const noexcept { return birthLimit_; }

size_t Cave::GetDeadLimit() const noexcept { return deadLimit_; }

double Cave::GetProbability() const noexcept { return probability_; }

size_t Cave::GetLength() const noexcept { return length_; }

size_t Cave::GetWidth() const noexcept { return width_; }

bool Cave::GetCell(size_t i, size_t j) const noexcept { return cave_[i][j]; }

std::vector<std::vector<bool>> Cave::GetCave() const noexcept { return cave_; }

size_t Cave::NumberOfLiveNeighbors(size_t i, size_t j) const {
  size_t number = 0;
  std::vector<std::pair<size_t, size_t>> neighbors = {
      {i - 1, j - 1}, {i - 1, j},     {i - 1, j + 1}, {i, j - 1},
      {i, j + 1},     {i + 1, j - 1}, {i + 1, j},     {i + 1, j + 1}};
  for (const auto& it : neighbors) {
    if ((it.first < 0 || it.first >= length_) ||
        (it.second < 0 || it.second >= width_)) {
      number += 1;
    } else {
      number += cave_[it.first][it.second];
    }
  }
  return number;
}

bool Cave::Update() {
  std::vector<std::vector<bool>> nextGen = cave_;
  for (size_t i = 0; i < length_; ++i) {
    for (size_t j = 0; j < width_; ++j) {
      if (cave_[i][j] == LIVE) {
        nextGen[i][j] =
            (NumberOfLiveNeighbors(i, j) < deadLimit_ ? DEAD : LIVE);
      } else {
        nextGen[i][j] =
            (NumberOfLiveNeighbors(i, j) > birthLimit_ ? LIVE : DEAD);
      }
    }
  }
  if (CompareState(cave_, nextGen) == false) {
    std::swap(cave_, nextGen);
    return true;
  }
  return false;
}

bool Cave::CompareState(const std::vector<std::vector<bool>>& prev,
                        const std::vector<std::vector<bool>>& next) const {
  size_t count = 0;
  for (size_t i = 0; i < length_; ++i) {
    for (size_t j = 0; j < width_; ++j) {
      if (prev[i][j] == next[i][j]) {
        ++count;
      }
    }
  }
  return count == prev.size() * prev[0].size();
}

void Cave::ReadFile(const std::string& path) {
  std::ifstream file(path);
  if (file.is_open()) {
    file >> length_ >> width_;
    AllocateMem();
    for (size_t i = 0; i < length_; ++i) {
      for (size_t j = 0; j < width_; ++j) {
        bool temp;
        file >> temp;
        cave_[i][j] = temp;
      }
    }
  }
  file.close();
}

void Cave::SaveFile(const std::string& path) const {
  std::ofstream out(path);
  out << length_ << " " << width_ << std::endl;
  for (size_t i = 0; i < length_; ++i) {
    for (size_t j = 0; j < width_; ++j) {
      out << cave_[i][j] << " ";
    }
    out << std::endl;
  }
  out.close();
}

bool Cave::Random() const {
  std::default_random_engine generator(std::random_device{}());
  std::bernoulli_distribution distribution(probability_);
  return distribution(generator);
}

void Cave::Generate() {
  for (size_t i = 0; i < length_; ++i) {
    for (size_t j = 0; j < width_; ++j) {
      cave_[i][j] = !Random();
    }
  }
}
}  // namespace awfl
