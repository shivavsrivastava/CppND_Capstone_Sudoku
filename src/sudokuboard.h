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
    void setCheckButton(bool setting) {
        std::lock_guard<std::mutex> lck(_mtx);
        _chkButton = setting;};

    bool getCheckButton() { 
        std::lock_guard<std::mutex> lck(_mtx);
        return _chkButton;};

    void setGenNewButton(bool setting) {
        std::lock_guard<std::mutex> lck(_mtx);
        _genNewButton=setting; };

    bool getGenNewButton() { 
        std::lock_guard<std::mutex> lck(_mtx);
        return _genNewButton;};

    void setSolButton(bool setting) {
        std::lock_guard<std::mutex> lck(_mtx);
        _solButton=setting; };

    bool getSolButton() { 
        std::lock_guard<std::mutex> lck(_mtx);
        return _solButton; };

    void setCurrentCellNum(int num) { 
        std::lock_guard<std::mutex> lck(_mtx);
        _currentCellNum = num;};

    int  getCurrentCellNum() {
        std::lock_guard<std::mutex> lck(_mtx);
        return _currentCellNum;}

    void setValidCell(bool setting) { 
        std::lock_guard<std::mutex> lck(_mtx);
        _currentCellValid = setting;};

    bool  getValidCell() {
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