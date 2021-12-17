#ifndef SOLVER_H
#define SOLVER_H
#include <iostream>
#include <vector>
#include <math.h>


typedef std::vector<std::vector<int>> board_t;

class SudokuSolver {

public:
  SudokuSolver();

  bool SetBoard(board_t& board);

  bool Solve();

  board_t GetBoard();


private:

  void MarkCell(int r, int c, int val, bool free);

  int GetFirstAvailableValFrom(int r, int c, int valStart);

  int Serialize(int r, int c);

  void Deserialize(int idx, int &r, int &c);

  int GetBlockIdx(int r, int c);

  int _N1;
  int _N2;
  
  board_t _board;

  std::vector<std::vector<bool>> _rowFree;
  std::vector<std::vector<bool>> _colFree;
  std::vector<std::vector<bool>> _blockFree;

};
#endif