#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "sudokucell.h"
#include "button.h"
#include "updater.h"
#include "sudokuscoreboard.h"

int main() {
  constexpr std::size_t kFramesPerSecond{30};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{720};
  constexpr std::size_t kScreenHeight{880};
  constexpr std::size_t kGridWidth{720};
  constexpr std::size_t kGridHeight{720};
  constexpr std::size_t kFontSize{32};
  constexpr std::size_t kSudokuSize{81};
  constexpr std::size_t kButtons{5};

  // Initialize all the 81 sudoku cells
  std::vector<std::shared_ptr<Cell>> grid;
  for (size_t i = 0; i < kSudokuSize; i++) {
    grid.push_back(std::make_shared<Cell>(i));
  }

  // other buttons on the board - level, stopwatch, check, solution, new
  std::vector<std::shared_ptr<Button>> buttons;
  for (size_t i = 0; i < kButtons; i++) {
      buttons.push_back(std::make_shared<Button>(i));
  }

  // Create MessageQueue
  std::shared_ptr<SudokuScoreBoard> boardPtr = std::make_shared<SudokuScoreBoard>();

  // Create other objects
  Controller controller;
  controller.setBoard(grid, buttons, boardPtr);

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight, kFontSize);
  renderer.setBoard(grid, buttons, boardPtr);

  Updater updater;
  updater.setBoard(grid, buttons, boardPtr);

  Game game(kGridWidth, kGridHeight);

  // Start the game!
  std::cout << " All instances of Game, controller and Renderer initialized!\n";
  game.play(controller, renderer, updater, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  grid.clear();
  buttons.clear();
  return 0;
}
