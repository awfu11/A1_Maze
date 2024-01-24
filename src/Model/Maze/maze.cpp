#include "maze.h"

namespace awfl {
Maze::Maze(const std::string& path) {
  std::ifstream file(path);
  if (file.is_open()) {
    file >> length_ >> width_;
    AllocateMem();

    for (size_t i = 0; i < length_; ++i) {
      for (size_t j = 0; j < width_; ++j) {
        bool temp;
        file >> temp;
        rightWall_[i][j] = temp;
      }
    }

    for (size_t i = 0; i < length_; ++i) {
      for (size_t j = 0; j < width_; ++j) {
        bool temp;
        file >> temp;
        bottomWall_[i][j] = temp;
      }
    }
  }
  file.close();
}

Maze::Maze(size_t length, size_t width) : length_(length), width_(width) {
  AllocateMem();
  Generate();
}

void Maze::AllocateMem() {
  rightWall_.resize(length_, std::vector<bool>(width_));
  bottomWall_.resize(length_, std::vector<bool>(width_));
}

void Maze::SaveFile(const std::string& path) const {
  std::ofstream out(path);
  out << length_ << " " << width_ << std::endl;
  for (size_t i = 0; i < length_; ++i) {
    for (size_t j = 0; j < width_; ++j) {
      out << rightWall_[i][j] << " ";
    }
    out << std::endl;
  }
  out << std::endl;
  for (size_t i = 0; i < length_; ++i) {
    for (size_t j = 0; j < width_; ++j) {
      out << bottomWall_[i][j] << " ";
    }
    out << std::endl;
  }
  out.close();
}

size_t Maze::GetLength() const noexcept { return length_; }

size_t Maze::GetWidth() const noexcept { return width_; }

std::pair<size_t, size_t> Maze::GetWalls(size_t i, size_t j) const noexcept {
  return {rightWall_[i][j], bottomWall_[i][j]};
}

std::vector<Coord> Maze::SolveMaze(Coord begin, Coord end) {
  counter_ = 1;
  solveMatrix_.clear();

  solveMatrix_.resize(length_, std::vector<size_t>(width_));
  solveMatrix_[begin.x][begin.y] = counter_;

  bool hasPath = true;
  while (solveMatrix_[end.x][end.y] == 0 && (hasPath = Wave())) {
    counter_++;
  }

  return FindPath(end, hasPath);
}

bool Maze::Wave() {
  bool flag = false;

  for (size_t i = 0; i < length_; ++i) {
    for (size_t j = 0; j < width_; ++j) {
      if (solveMatrix_[i][j] == counter_) {
        flag = true;
        if (j < width_ - 1 && !rightWall_[i][j] && !solveMatrix_[i][j + 1]) {
          solveMatrix_[i][j + 1] = counter_ + 1;
        }
        if (j > 0 && !rightWall_[i][j - 1] && !solveMatrix_[i][j - 1]) {
          solveMatrix_[i][j - 1] = counter_ + 1;
        }
        if (i > 0 && !bottomWall_[i - 1][j] && !solveMatrix_[i - 1][j]) {
          solveMatrix_[i - 1][j] = counter_ + 1;
        }
        if (i < length_ - 1 && !bottomWall_[i][j] && !solveMatrix_[i + 1][j]) {
          solveMatrix_[i + 1][j] = counter_ + 1;
        }
      }
    }
  }

  return flag;
}

std::vector<Coord> Maze::FindPath(Coord pos, bool hasPath) const {
  std::vector<Coord> path;
  size_t count = counter_;

  if (hasPath) {
    path.push_back({pos.x, pos.y});
  }

  while (count != 1 && hasPath) {
    count--;
    if (pos.y < width_ - 1 && solveMatrix_[pos.x][pos.y + 1] == count &&
        !rightWall_[pos.x][pos.y]) {
      ++pos.y;
    } else if (pos.y > 0 && solveMatrix_[pos.x][pos.y - 1] == count &&
               !rightWall_[pos.x][pos.y - 1]) {
      --pos.y;
    } else if (pos.x > 0 && solveMatrix_[pos.x - 1][pos.y] == count &&
               !bottomWall_[pos.x - 1][pos.y]) {
      --pos.x;
    } else if (pos.x < length_ - 1 && solveMatrix_[pos.x + 1][pos.y] == count &&
               !bottomWall_[pos.x][pos.y]) {
      ++pos.x;
    }
    path.push_back({pos.x, pos.y});
  }

  return path;
}

void Maze::Generate() {
  counter_ = 1;
  FillEmptyValue();
  for (size_t i = 0; i < length_ - 1; ++i) {
    AssignUniqueSet();
    AddingVerticalWalls(i);
    AddingHorizontalWalls(i);
    CheckedHorizontalWalls(i);
    PreparatingNewLine(i);
  }
  AddingEndLine();
  sideLine_.clear();
}

void Maze::FillEmptyValue() {
  for (size_t i = 0; i < width_; ++i) {
    sideLine_.push_back(0);
  }
}

void Maze::AssignUniqueSet() {
  for (size_t i = 0; i < width_; ++i) {
    if (sideLine_[i] == 0) {
      sideLine_[i] = counter_++;
    }
  }
}

void Maze::AddingVerticalWalls(size_t row) {
  for (size_t i = 0; i < width_ - 1; ++i) {
    bool choise = Random();
    if (choise || sideLine_[i] == sideLine_[i + 1]) {
      rightWall_[row][i] = choise;
    } else {
      MergeSet(i, sideLine_[i]);
    }
  }
  rightWall_[row][width_ - 1] = true;
}

bool Maze::Random() const {
  std::default_random_engine generator(std::random_device{}());
  std::bernoulli_distribution distribution(0.5);
  return distribution(generator);
}

void Maze::MergeSet(size_t idx, int elem) {
  int mutableSet = sideLine_[idx + 1];
  for (size_t i = 0; i < width_; ++i) {
    if (sideLine_[i] == mutableSet) {
      sideLine_[i] = elem;
    }
  }
}

void Maze::AddingHorizontalWalls(size_t row) {
  for (size_t i = 0; i < width_; ++i) {
    bool choise = Random();
    if (CalculateUniqueSet(sideLine_[i]) != 1 && choise) {
      bottomWall_[row][i] = true;
    }
  }
}

int Maze::CalculateUniqueSet(int elem) {
  int countUniqSet = 0;
  for (size_t i = 0; i < width_; i++) {
    if (sideLine_[i] == elem) {
      ++countUniqSet;
    }
  }
  return countUniqSet;
}

void Maze::CheckedHorizontalWalls(size_t row) {
  for (size_t i = 0; i < width_; ++i) {
    if (CalculateHorizontalWalls(sideLine_[i], row) == 0) {
      bottomWall_[row][i] = false;
    }
  }
}

int Maze::CalculateHorizontalWalls(int elem, size_t row) {
  int countHorizontalWalls = 0;
  for (size_t i = 0; i < width_; ++i) {
    if (sideLine_[i] == elem && bottomWall_[row][i] == false) {
      countHorizontalWalls++;
    }
  }
  return countHorizontalWalls;
}

void Maze::PreparatingNewLine(size_t row) {
  for (size_t i = 0; i < width_; ++i) {
    if (bottomWall_[row][i] == true) {
      sideLine_[i] = 0;
    }
  }
}

void Maze::AddingEndLine() {
  AssignUniqueSet();
  AddingVerticalWalls(length_ - 1);
  CheckedEndLine();
}

void Maze::CheckedEndLine() {
  for (size_t i = 0; i < width_ - 1; ++i) {
    if (sideLine_[i] != sideLine_[i + 1]) {
      rightWall_[length_ - 1][i] = false;
      MergeSet(i, sideLine_[i]);
    }
    bottomWall_[length_ - 1][i] = true;
  }
  bottomWall_[length_ - 1][width_ - 1] = true;
}
}  // namespace awfl
