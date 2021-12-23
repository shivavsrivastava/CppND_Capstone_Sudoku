#include "sudokusolver.h"

SudokuSolver::SudokuSolver() {
    _N1 = 3;
    _N2 = 9;
}

bool SudokuSolver::setBoard(board_t& board) {
    _board = board;
    
    // Reset row, col, block and diag trackers
    _sol.resize(_N2,std::vector<int>(_N2,0));
    _rowFree.resize(_N2,std::vector<bool>(_N2,true));
    _colFree.resize(_N2,std::vector<bool>(_N2,true));
    _blockFree.resize(_N2,std::vector<bool>(_N2,true));

    // Populate row, col, block trackers
    for (int r = 0; r < _N2; r++) {
        for (int c = 0; c < _N2; c++) {
          int val = board[r][c];
          _sol[r][c] = val;
          if (val != 0) markCell(r,c,val,false);
        }
    }
    return true;
}

board_t SudokuSolver::getBoard() {
    return _sol;
}

bool SudokuSolver::solve() {
    std::vector<int> searchPath; // True if searchable (max 81)
    int r,c;
    for (int i=0; i< _N2*_N2; i++) {
        deserialize(i,r,c);
        if (_sol[r][c] == 0) {
          searchPath.push_back(i);
        }
    }
    std::vector<int>::iterator searchPathIt = searchPath.begin();

    int iterCount = 0;
    while(searchPathIt != searchPath.end()) {
        iterCount++;
        deserialize(*searchPathIt,r,c);

        #ifdef DEBUG1
        std::cout << "Checking cell index " << *searchPathIt << " (" << r << ", " << c << ")\n";
        #endif

        int curVal = _sol[r][c];
        int nextVal = getFirstAvailableValFrom(r,c,curVal+1);

        #ifdef DEBUG1
        std::cout << "Next value: " << nextVal << "\n";
        #endif
        
        if (nextVal == 0) { // backtrack
            if (searchPathIt == searchPath.begin()) {
                return false; // no solution found
            }

            // Mark cell as empty
            _sol[r][c] = 0;

            // Move backwards to previous cell on search path
            searchPathIt--;

            // Collect row col indicies for previous cell
            deserialize(*searchPathIt,r,c);

            // Refree previous val
            int val = _sol[r][c];
            markCell(r,c,val,true); 

            #ifdef DEBUG1
            std::cout << "Backtracking \n";
            #endif
        }
        else { // go forwards
          
            #ifdef DEBUG1
            std::cout << "Placing " << nextVal << " at (" << r << ", " <<
            c << ") \n";
            #endif

            // Mark board with trial val
            _sol[r][c] = nextVal;

            // Mark free arrays with this cell candidate
            markCell(r,c,nextVal,false);

            // Move to next cell
            searchPathIt++;
        }
    }


    #ifdef DEBUG1
    std::cout << "Solved in " << iterCount << " iterations \n";
    #endif

    return true;
}

void SudokuSolver::markCell(int r, int c, int val, bool free) {
    _rowFree[r][val-1] = free;
    _colFree[c][val-1] = free;
    _blockFree[getBlockIdx(r,c)][val-1] = free;
}

int SudokuSolver::getBlockIdx(int r, int c) {
    int blockR = r / _N1 ;
    int blockC = c / _N1 ;
    return blockR*_N1 + blockC;
}

int SudokuSolver::getFirstAvailableValFrom(int r, int c, int valStart = 1) {
    for (size_t val = valStart; val <= _N2; val++) {
        if (_rowFree[r][val-1] &&
            _colFree[c][val-1] &&
            _blockFree[getBlockIdx(r,c)][val-1]) {
            return val;
        }
    }
    return 0;
}

int SudokuSolver::serialize(int r, int c) {
    return _N2 * r + c;
}

void SudokuSolver::deserialize(int idx, int &r, int &c) {
    r = idx/_N2;
    c = idx % _N2;
}

