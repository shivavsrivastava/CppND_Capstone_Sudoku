#ifndef UPDATER_H
#define UPDATER_H

#include <random>
#include <vector>
#include <memory>
#include "controller.h"
#include "sudokusolver.h"
#include "button.h"
#include "sudokucell.h"
#include "sudokuscoreboard.h"


class SudokuSolver;
class SudokuBoard;

class Updater {
 public:
  Updater();
  ~Updater();
  bool loadBoard(int fileNum);
  void run();
  void setBoard(std::vector<std::shared_ptr<Cell>>& grid, 
                std::vector<std::shared_ptr<Button>>& buttons, 
                std::shared_ptr<SudokuScoreBoard>& boardPtr);
    
  void stop();

 private:
  bool solve();
  bool readyToStop();
  void loadSolvedBoard();

  bool _stop;
  std::mutex _stop_mtx;
  std::vector<std::shared_ptr<Cell>>         _grid;
  std::vector<std::shared_ptr<Button>>       _buttons;
  std::shared_ptr<SudokuScoreBoard>          _boardPtr;
  std::vector<std::vector<int>>              _board;
  std::vector<std::vector<int>>              _solution;
  std::unique_ptr<SudokuSolver>              _solver;
};

#endif