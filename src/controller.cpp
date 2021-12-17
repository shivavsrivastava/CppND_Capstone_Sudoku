#include <iostream>
#include <SDL.h>
#include "controller.h"

Controller::Controller() : _stop(false), _currCellNumber(0) {}

Controller::~Controller() {
  Stop();
}

void Controller::Stop() {
  std::lock_guard<std::mutex> lck(_stop_mtx);
  _stop = true;
}

bool Controller::ReadyToStop() {
  std::lock_guard<std::mutex> lck(_stop_mtx);
  return _stop;
}

void Controller::HandleInput() {
  SDL_Event e;
  while (!ReadyToStop()) {
    while(SDL_PollEvent(&e) != 0) {
      //std::cout << "Controller::HandleInput Event seen! \n";
      if (e.type == SDL_QUIT) {
        std::lock_guard<std::mutex> lck(_stop_mtx);
        _stop = true;
      }
      else {
        for (auto it: _grid) {
          if(it->IsEditable()) {
            if(it->GetMouseEvent(&e) == ButtonState::BUTTON_MOUSE_DOWN) {
              std::cout << "Controller::HandleInput Button clicked in grid \n";
              it->SetSelected(false);
              _currCellNumber = it->GetID();
              it->SetSelected(true);
            }
          }
        }
        // first 2 buttons are level, stopwatch (so not editable)
        // next 3 are check, solution, new
        for (auto it: _buttons) {
          if(it->GetMouseEvent(&e) == ButtonState::BUTTON_MOUSE_DOWN) {
            switch (it->GetID()) {
              case (2):
                _boardPtr->SetCheckButton(true);
                std::cout << "Controller::HandleInput CheckButton clicked \n";
                break;
              case(3):
                _boardPtr->SetGenNewButton(true);
                std::cout << "Controller::HandleInput GenNew clicked \n";
                break;
              case (4):
                _boardPtr->SetSolButton(true);
                std::cout << "Controller::HandleInput SolutionButton clicked \n";
                break;
            }
          }
        }
        _grid.at(_currCellNumber)->HandleKeyboardEvent(&e);
        _grid.at(_currCellNumber)->SetSelected(false);
        _boardPtr->SetValidCell(true);
        _boardPtr->SetCurrentCellNum(_currCellNumber);
      }

      // Free CPU
      //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }
  std::cout << "Exit thread: Controller::HandleInput\n";
}

void Controller::SetBoard(std::vector<std::shared_ptr<Cell>>& grid, 
                std::vector<std::shared_ptr<Button>>& buttons,
                std::shared_ptr<SudokuBoard>& boardPtr){
                  _grid = grid; 
                  _buttons = buttons; 
                  _boardPtr = boardPtr;
                }