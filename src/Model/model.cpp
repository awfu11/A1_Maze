#include "model.h"

namespace awfl {
Model::~Model() {
  delete maze_;
  delete cave_;
}

void Model::MazeLoadFile(const std::string& path) {
  if (maze_ != nullptr) {
    delete maze_;
  }
  maze_ = new Maze(path);
}

void Model::MazeSaveFile(const std::string& path) const {
  if (maze_ != nullptr) {
    maze_->SaveFile(path);
  }
}

void Model::MazeGenerate(size_t length, size_t width) {
  if (maze_ != nullptr) {
    delete maze_;
  }
  maze_ = new Maze(length, width);
}

std::pair<size_t, size_t> Model::MazeGetWalls(size_t i,
                                              size_t j) const noexcept {
  if (maze_ != nullptr) {
    return maze_->GetWalls(i, j);
  }
  return {0, 0};
}

std::pair<size_t, size_t> Model::MazeGetSize() const noexcept {
  if (maze_ != nullptr) {
    return {maze_->GetLength(), maze_->GetWidth()};
  }
  return {0, 0};
}

std::vector<Coord> Model::MazeSolve(std::pair<size_t, size_t> from,
                                    std::pair<size_t, size_t> to) {
  if (maze_ != nullptr) {
    return maze_->SolveMaze(from, to);
  }
  return {};
}

void Model::CaveSetProbability(double probability) {
  if (cave_ != nullptr) {
    cave_->SetProbability(probability);
  }
}

void Model::CaveSetBirthLimit(int birthLimit) {
  if (cave_ != nullptr) {
    cave_->SetBirthLimit(birthLimit);
  }
}

void Model::CaveSetDeathLimit(int deadLimit) {
  if (cave_ != nullptr) {
    cave_->SetDeadLimit(deadLimit);
  }
}

void Model::CaveLoadFile(const std::string& path) {
  if (cave_ != nullptr) {
    delete cave_;
  }
  cave_ = new Cave(path);
}

void Model::CaveSaveFile(const std::string& path) const {
  if (cave_ != nullptr) {
    cave_->SaveFile(path);
  }
}

void Model::CaveGenerate(size_t length, size_t width, double probability) {
  if (cave_ != nullptr) {
    delete cave_;
  }
  cave_ = new Cave(length, width, probability);
}

bool Model::CaveGetCell(size_t i, size_t j) const noexcept {
  return (cave_ ? cave_->GetCell(i, j) : false);
}

std::pair<size_t, size_t> Model::CaveGetSize() const noexcept {
  if (cave_ != nullptr) {
    return {cave_->GetLength(), cave_->GetWidth()};
  }
  return {0, 0};
}

bool Model::CaveUpdate() {
  if (cave_ != nullptr) {
    return cave_->Update();
  }
  return false;
}
}  // namespace awfl
