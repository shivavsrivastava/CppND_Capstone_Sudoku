#include <iostream>
#include <SDL.h>
#include "controller.h"

Controller::Controller() : _stop(false), _currCellNumber(0) {}

Controller::~Controller() {
  stop();
}

void Controller::stop() {
  std::lock_guard<std::mutex> lck(_stop_mtx);
  _stop = true;
}

bool Controller::readyToStop() {
  std::lock_guard<std::mutex> lck(_stop_mtx);
  return _stop;
}

void Controller::handleInput() {
  SDL_Event e;
  while (!readyToStop()) {
    while(SDL_PollEvent(&e) != 0) {
      //std::cout << "Controller::HandleInput Event seen! \n";
      if (e.type == SDL_QUIT || (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)) {
        std::lock_guard<std::mutex> lck(_stop_mtx);
        _stop = true;
      }
      else {
        for (auto it: _grid) {
          if(it->isEditable()) {
            it->setSelected(false);
            if(it->getMouseEvent(&e) == ButtonState::BUTTON_MOUSE_DOWN) {
              std::cout << "Controller::HandleInput Button clicked in grid \n";
              _currCellNumber = it->getId();
              it->setSelected(true);
            }
          }
        }
        // first 2 buttons are level, stopwatch (so not editable)
        // next 3 are check, solution, new
        for (auto it: _buttons) {
          if(it->getMouseEvent(&e) == ButtonState::BUTTON_MOUSE_DOWN) {
            switch (it->getId()) {
              case (2):
                if(_boardPtr->getCheckButton()) {
                  // Check button was clicked again (click again to clear)
                  _boardPtr->setCheckButton(false);
                }
                else
                  _boardPtr->setCheckButton(true);
                std::cout << "Controller::HandleInput CheckButton clicked \n";
                // reset the solution button
                _boardPtr->setSolButton(false);
                break;
              case(3):
                if(_boardPtr->getSolButton()) {
                  // Solution button was clicked again (click again to clear)
                  _boardPtr->setSolButton(false);
                }
                else
                  _boardPtr->setSolButton(true);
                std::cout << "Controller::HandleInput SolutionButton clicked \n";
                // reset the check button
                _boardPtr->setCheckButton(false);
                break;
              case (4):
                _boardPtr->setGenNewButton(true);
                std::cout << "Controller::HandleInput GenNew clicked \n";
                // reset the solution button and check button
                _boardPtr->setSolButton(false);
                _boardPtr->setCheckButton(false);
                break;
            }
          }
        }
        _grid.at(_currCellNumber)->handleKeyboardEvent(&e);
        _grid.at(_currCellNumber)->setSelected(false);
        _boardPtr->setValidCell(true);
        _boardPtr->setCurrentCellNum(_currCellNumber);
      }

      // Free CPU
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }
  std::cout << "Exit thread: Controller::HandleInput\n";
}

void Controller::setBoard(std::vector<std::shared_ptr<Cell>>& grid, 
                std::vector<std::shared_ptr<Button>>& buttons,
                std::shared_ptr<SudokuScoreBoard>& boardPtr){
                  _grid = grid; 
                  _buttons = buttons; 
                  _boardPtr = boardPtr;
                }