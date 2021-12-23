/* Author      : Shiva Srivastava
 * Description : Sudoku Solver Class.
 *               The solver uses backtracking algorithm.
 * 
 */
#ifndef SOLVER_H
#define SOLVER_H
#include <iostream>
#include <vector>
#include <math.h>
#include <mutex>


typedef std::vector<std::vector<int>> board_t;

class SudokuSolver {

public:
  SudokuSolver();

  bool setBoard(board_t& board);

  bool solve();

  board_t getBoard();


private:

  void markCell(int r, int c, int val, bool free);

  int getFirstAvailableValFrom(int r, int c, int valStart);

  int serialize(int r, int c);

  void deserialize(int idx, int &r, int &c);

  int getBlockIdx(int r, int c);


  int                             _N1;
  int                             _N2;
  
  board_t                         _board;

  std::vector<std::vector<bool>> _rowFree;
  std::vector<std::vector<bool>> _colFree;
  std::vector<std::vector<bool>> _blockFree;

};
#endif