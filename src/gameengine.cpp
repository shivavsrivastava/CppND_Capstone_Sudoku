#include <iostream>
#include <fstream>
#include <chrono>
#include "SDL.h"
#include "SDL_ttf.h"
#include "gameengine.h"


GameEngine::GameEngine() {
  _stop       = false;
  _solver     = std::make_unique<SudokuSolver>();
}

GameEngine::~GameEngine() {
  Stop();
}

void GameEngine::Run() {
  bool init = false;
  while(!ReadyToStop()) {
    if(!init) {
      // Run the initial load board logic
      LoadBoard(9); // start with file number 1
      Solve();
      LoadSolvedBoard();
    }
    else { // init = true
      //std::cout << "GameEngine::Run - entered loop, will get message from MessageQ \n";
      if(_boardPtr->GetCheckButton()) {
        // logic to check 
      }
      if(_boardPtr->GetGenNewButton()) {
        // logic to generate new sudoku and set the level button too
      }
      if(_boardPtr->GetSolButton()) {
        // Renderer has to display solution
      }
      if(_boardPtr->GetValidCell()) {
        int cellIndex = _boardPtr->GetCurrentCellNum();
      }
    }
    // Set init to true, so that first logic is not visited again
    init = true; 
    // Free CPU
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  std::cout << "Exit thread: GameEngine::Run\n";
}

void GameEngine::Stop() {
  std::lock_guard<std::mutex> lck(_stop_mtx);
  _stop = true;
}

bool GameEngine::ReadyToStop() {
  std::lock_guard<std::mutex> lck(_stop_mtx);
  return _stop;
}

bool GameEngine::LoadBoard(int fileNumber) {
  int N = _grid.size(); // default to 81
  int N1 = sqrt(N);
  _board.resize(N1, std::vector<int>(N1));
  //std::cout << "Loading Board in GameEngine : N= " << N << " N1=" << N1 << "\n";
  std::string fileName;
  if(fileNumber>=1 && fileNumber <=3)
    fileName = "../resources/beginner" + std::to_string(fileNumber) + ".txt";
  else if(fileNumber>=4 && fileNumber <=6)
    fileName = "../resources/advanced" + std::to_string(fileNumber) + ".txt";
  else // expert 
    fileName = "../resources/expert" + std::to_string(fileNumber) + ".txt";  
  
  std::ifstream fp(fileName, std::ifstream::in);
  if (! fp) {
    std::cerr << "Error, file couldn't be opened \n"; 
  }
  //std::cout << "LoadBoard: game 1 found\n";
  int val, index=0;
  for(int r = 0; r < N1; r++) {
    for(int c = 0; c < N1; c++){
      fp >> val;
      //std::cout << "LoadBoard: val = " << val << "\n";
      _board[r][c] = val;
      // Set the grid directly
      index = r*N1 + c;
      _grid[index]->SetNumber(val);
      if(val ==0)
        _grid[index]->SetEditable(true);
      else
        _grid[index]->SetEditable(false);
      
      // if the board is not filled and there is no input
      if ( !fp ) {
        std::cerr << "Error reading file for element (" << r << ", " << c << ")\n"; 
        return false; 
      }
    }
  }
  std::cout << "Board Loaded with game 1\n";
  return true;
}


void GameEngine::LoadSolvedBoard() {
  int N = _grid.size(); // default to 81
  int N1 = sqrt(N);
  int index;
  for(int r = 0; r < N1; r++) {
    for(int c = 0; c < N1; c++){
      index = r*N1 + c;
      _grid.at(index)->SetSolution(_solution[r][c]);
      
    }
  }
}

bool GameEngine::Solve() {
  _solver->SetBoard(_board);

  using namespace std::chrono;
  steady_clock::time_point t1 = steady_clock::now();
  bool success = _solver->Solve();
  steady_clock::time_point t2 = steady_clock::now();
  if(!success) {
    std::cerr << "Error! Puzzle not solvable. \n";
  }
  else {
    std::cout << "Puzzle solved. \n";
    duration<double> timeSpan = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Solve time: " << timeSpan.count()*1000 << " ms \n";
    _solution = _solver->GetBoard();
    LoadSolvedBoard();
  }
  return success;
}

void GameEngine::SetBoard(std::vector<std::shared_ptr<Cell>>& grid, 
                std::vector<std::shared_ptr<Button>>& buttons,
                std::shared_ptr<SudokuBoard>& boardPtr)
        {_grid = grid; _buttons = buttons; _boardPtr = boardPtr;}