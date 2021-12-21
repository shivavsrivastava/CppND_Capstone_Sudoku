#include <iostream>
#include <fstream>
#include <chrono>
#include "SDL.h"
#include "SDL_ttf.h"
#include "updater.h"


Updater::Updater(int seed, int totalGames) {
  _seed       = seed;
  _totalGames = totalGames;
  _stop       = false;
  _lastGameSolved = 1;
  _solver     = nullptr;
}

Updater::~Updater() {
  stop();
}

void Updater::run() {
  bool init = false;
  // seed the random generator
  srand(_seed);
  while(!readyToStop()) {
    if(!init) {
      // Run the initial load board logic
      int newGame = 1;
      loadBoard(newGame); // start with file number 1
      solve();
      loadSolvedBoardAndGrid();
      _boardPtr->setGameLevel(newGame);
      _boardPtr->setGenNewButton(false);
      _lastGameSolved = newGame;
    }
    else { // init = true
      //std::cout << "Updater::Run - entered loop, will get message from MessageQ \n";
      if(_boardPtr->getCheckButton()) {
        // logic to check 
      }
      if(_boardPtr->getGenNewButton()) {
        // logic to generate new sudoku and set the level button too
        int newGame = rand()%_totalGames + 1;
        while(newGame == _lastGameSolved) 
          newGame = rand()%_totalGames + 1;
        #ifdef DEBUG1
        std::cout << "New game will be loaded, new game number= " << newGame << "\n";
        #endif
        _boardPtr->setGameLevel(newGame);
        loadBoard(newGame);
        solve();
        loadSolvedBoardAndGrid();
        _boardPtr->setGenNewButton(false);
        _lastGameSolved = newGame;
      }
      if(_boardPtr->getSolButton()) {
        // Renderer has to display solution
      }
      if(_boardPtr->getValidCell()) {
        int cellIndex = _boardPtr->getCurrentCellNum();
      }
    }
    // Set init to true, so that first logic is not visited again
    init = true; 
    // Free CPU
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  std::cout << "Exit thread: Updater::Run\n";
}

void Updater::stop() {
  std::lock_guard<std::mutex> lck(_stop_mtx);
  _stop = true;
}

bool Updater::readyToStop() {
  std::lock_guard<std::mutex> lck(_stop_mtx);
  return _stop;
}

bool Updater::loadBoard(int fileNumber) {
  int N = _grid.size(); // default to 81
  int N1 = sqrt(N);
  _board.resize(N1, std::vector<int>(N1, 0));

  //std::cout << "Loading Board in Updater : N= " << N << " N1=" << N1 << "\n";
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
      // if the board is not filled and there is no input
      if ( !fp ) {
        std::cerr << "Error reading file for element (" << r << ", " << c << ")\n"; 
        return false; 
      }
    }
  }
  std::cout << "Board Loaded with game " << fileNumber << "\n";
  return true;
}


void Updater::loadSolvedBoardAndGrid() {
  int N = _grid.size(); // default to 81
  int N1 = sqrt(N);
  int index;
  for(int r = 0; r < N1; r++) {
    for(int c = 0; c < N1; c++){
      index = r*N1 + c;
      _grid.at(index)->setNumber(_board[r][c]);
      if(_board[r][c]==0)
        _grid.at(index)->setEditable(true);
      else
        _grid.at(index)->setEditable(false);
      _grid.at(index)->setSolution(_solution[r][c]);
      
    }
  }
}

bool Updater::solve() {
  _solver     = std::make_unique<SudokuSolver>();
  _solver->setBoard(_board);

  using namespace std::chrono;
  steady_clock::time_point t1 = steady_clock::now();
  bool success = _solver->solve();
  steady_clock::time_point t2 = steady_clock::now();
  if(!success) {
    std::cerr << "Error! Puzzle not solvable. \n";
  }
  else {
    std::cout << "Puzzle solved. \n";
    duration<double> timeSpan = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Solve time: " << timeSpan.count()*1000 << " ms \n";
    _solution = _solver->getBoard();
    loadSolvedBoardAndGrid();
  }
  return success;
}

void Updater::setBoard(std::vector<std::shared_ptr<Cell>>& grid, 
                std::vector<std::shared_ptr<Button>>& buttons,
                std::shared_ptr<SudokuScoreBoard>& boardPtr)
        {_grid = grid; _buttons = buttons; _boardPtr = boardPtr;}