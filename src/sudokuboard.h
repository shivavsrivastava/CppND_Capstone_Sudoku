#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H
#include <mutex>

class SudokuBoard {
public:
    SudokuBoard() {
        _chkButton = false;
        _genNewButton = false;
        _solButton    = false;
        _currentCellNum = 0;

    }
    // getter and setter functions
    void SetCheckButton(bool setting) {
        std::lock_guard<std::mutex> lck(_mtx);
        _chkButton = setting;};

    bool GetCheckButton() { 
        std::lock_guard<std::mutex> lck(_mtx);
        return _chkButton;};

    void SetGenNewButton(bool setting) {
        std::lock_guard<std::mutex> lck(_mtx);
        _genNewButton=setting; };

    bool GetGenNewButton() { 
        std::lock_guard<std::mutex> lck(_mtx);
        return _genNewButton;};

    void SetSolButton(bool setting) {
        std::lock_guard<std::mutex> lck(_mtx);
        _solButton=setting; };

    bool GetSolButton() { 
        std::lock_guard<std::mutex> lck(_mtx);
        return _solButton; };

    void SetCurrentCellNum(int num) { 
        std::lock_guard<std::mutex> lck(_mtx);
        _currentCellNum = num;};

    int  GetCurrentCellNum() {
        std::lock_guard<std::mutex> lck(_mtx);
        return _currentCellNum;}

    void SetValidCell(bool setting) { 
        std::lock_guard<std::mutex> lck(_mtx);
        _currentCellValid = setting;};

    bool  GetValidCell() {
        std::lock_guard<std::mutex> lck(_mtx);
        return _currentCellValid;}

private:
    bool _chkButton;
    bool _genNewButton;
    bool _solButton;
    int  _currentCellNum;
    bool _currentCellValid;
    std::mutex _mtx; 
};

#endif