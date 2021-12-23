#include <iostream>
#include <chrono>
#include "controller.h"

Controller::Controller() : _stop(false), _currCellNumber(0) {}

Controller::~Controller()
{
    stop();
}

void Controller::stop()
{
    std::lock_guard<std::mutex> lck(_stop_mtx);
    _stop = true;
}

bool Controller::readyToStop()
{
    std::lock_guard<std::mutex> lck(_stop_mtx);
    return _stop;
}

void Controller::handleInput()
{
    SDL_Event e;
    while (!readyToStop())
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT || (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE))
            {
                std::lock_guard<std::mutex> lck(_stop_mtx);
                _stop = true;
            }
            else
            { // Handle grid of cells
                for (auto it : _grid)
                {
                    if (it->isEditable())
                    {
                        it->setSelected(false);
                        if (it->getMouseEvent(&e) == ButtonState::BUTTON_MOUSE_DOWN)
                        {
#ifdef DEBUG1
                            std::cout << "Controller::HandleInput Button clicked in grid \n";
#endif
                            _currCellNumber = it->getId();
                            it->setSelected(true);
                        }
                    }
                }
                // Handle buttons
                // first button is level and not editable
                // next 3 are check, solution, new
                for (auto it : _buttons)
                {
                    if (it->getMouseEvent(&e) == ButtonState::BUTTON_MOUSE_DOWN)
                    {
                        switch (it->getId())
                        {
                        case (1):
                            if (_boardPtr->getCheckButton())
                            {
                                // Check button was clicked again (click again to clear)
                                _boardPtr->setCheckButton(false);
                            }
                            else
                                _boardPtr->setCheckButton(true);
#ifdef DEBUG1
                            std::cout << "Controller::HandleInput CheckButton clicked \n";
#endif
                            // reset the solution button
                            _boardPtr->setSolButton(false);
                            break;
                        case (2):
                            if (_boardPtr->getSolButton())
                            {
                                // Solution button was clicked again (click again to clear)
                                _boardPtr->setSolButton(false);
                            }
                            else
                                _boardPtr->setSolButton(true);
#ifdef DEBUG1
                            std::cout << "Controller::HandleInput SolutionButton clicked \n";
#endif
                            // reset the check button and complete state
                            _boardPtr->setCheckButton(false);
                            _boardPtr->setSudokuComplete(false);
                            break;
                        case (3):
                            _boardPtr->setGenNewButton(true);
#ifdef DEBUG1
                            std::cout << "Controller::HandleInput GenNew clicked \n";
#endif
                            // reset the solution button and check button
                            _boardPtr->setSolButton(false);
                            _boardPtr->setCheckButton(false);
                            _boardPtr->setSudokuComplete(false);
                            break;
                        }
                    }
                }
                _grid.at(_currCellNumber)->handleKeyboardEvent(&e);
                _grid.at(_currCellNumber)->setSelected(false);
            }

            // Free CPU
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    std::cout << "Exit thread: Controller::HandleInput \n";
}

void Controller::setBoard(std::vector<std::shared_ptr<Cell>> &grid,
                          std::vector<std::shared_ptr<Button>> &buttons,
                          std::shared_ptr<SudokuScoreBoard> &boardPtr)
{
    _grid = grid;
    _buttons = buttons;
    _boardPtr = boardPtr;
}