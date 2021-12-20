#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <mutex>
#include <vector>
#include <memory>
#include <thread>
#include "button.h"
#include "sudokucell.h"
#include "sudokuboard.h"

class SudokuBoard;

class Controller {
public:
  Controller();
  ~Controller();

  void handleInput();
  void setBoard(std::vector<std::shared_ptr<Cell>>& grid, 
                std::vector<std::shared_ptr<Button>>& buttons,
                std::shared_ptr<SudokuBoard>& boardPtr);
  
  void stop();
  bool readyToStop();

private:

  bool _stop;
  std::mutex _stop_mtx;
  std::vector<std::shared_ptr<Cell>>         _grid;
  std::vector<std::shared_ptr<Button>>       _buttons;
  std::shared_ptr<SudokuBoard>               _boardPtr;
  int                                        _currCellNumber;
};

#endif