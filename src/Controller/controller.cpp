#include "controller.h"

namespace awfl {
Controller::Controller(View* view, Model* model) : view_(view), model_(model) {
  view_->show();

  MazeInitConnections();
  CaveInitConnections();
}

void Controller::MazeInitConnections() {
  connect(view_, qOverload<const std::string&>(&View::MazeLoadFile), model_, [&](const std::string& path) {
    model_->MazeLoadFile(path);
  });

  connect(view_, qOverload<const std::string&>(&View::MazeSaveFile), model_, [&](const std::string& path) {
    model_->MazeSaveFile(path);
  });

  connect(view_, qOverload<size_t, size_t>(&View::MazeGenerate), model_, [&](size_t length, size_t width) {
    model_->MazeGenerate(length, width);
  });

  connect(view_, qOverload<std::pair<size_t, size_t>&, size_t, size_t>(&View::MazeGetWalls), model_,
      [&](std::pair<size_t, size_t>& walls, size_t i, size_t j) { walls = model_->MazeGetWalls(i, j); });

  connect( view_, qOverload<std::pair<size_t, size_t>&>(&View::MazeGetSize), model_,
      [&](std::pair<size_t, size_t>& size) { size = model_->MazeGetSize(); });

  connect(view_, qOverload<std::vector<Coord>&, std::pair<size_t, size_t>, std::pair<size_t, size_t>>(&View::MazeSolve), model_,
      [&](std::vector<Coord>& path, std::pair<size_t, size_t> from, std::pair<size_t, size_t> to) {
            path = model_->MazeSolve(from, to);
          });
}

void Controller::CaveInitConnections() {
  connect(view_, qOverload<const std::string&>(&View::CaveLoadFile), model_, [&](const std::string& path) {
    model_->CaveLoadFile(path);
  });

  connect(view_, qOverload<const std::string&>(&View::CaveSaveFile), model_, [&](const std::string& path) {
    model_->CaveSaveFile(path);
  });

  connect(view_, qOverload<size_t, size_t, double>(&View::CaveGenerate), model_,
      [&](size_t length, size_t width, double probability) {
        model_->CaveGenerate(length, width, probability);
      });

  connect(view_, qOverload<int>(&View::CaveSetBirthLimit), model_, [&](int birthLimit) {
    model_->CaveSetBirthLimit(birthLimit);
  });
  connect(view_, qOverload<int>(&View::CaveSetDeathLimit), model_, [&](int deadLimit) {
    model_->CaveSetDeathLimit(deadLimit);
  });

  connect(view_, qOverload<bool&>(&View::CaveUpdateState), model_, [&](bool& state) {
    state = model_->CaveUpdate();
  });

  connect(view_, qOverload<bool&, size_t, size_t>(&View::CaveGetCell), model_, [&](bool& cell, size_t i, size_t j) {
    cell = model_->CaveGetCell(i, j);
  });

  connect( view_, qOverload<std::pair<size_t, size_t>&>(&View::CaveGetSize), model_, [&](std::pair<size_t, size_t>& size) {
    size = model_->CaveGetSize();
  });
}
}  // namespace awfl
