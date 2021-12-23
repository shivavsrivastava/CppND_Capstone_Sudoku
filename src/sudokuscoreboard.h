/* Author      : Shiva Srivastava
 * Description : SCoreboard Class
 *               This class is like a buffer class that is accessed by
 *               controller, updater and renderer.
 * 
 */
#ifndef SUDOKUSCOREBOARD_H
#define SUDOKUSCOREBOARD_H
#include <mutex>

class SudokuScoreBoard {
public:
    SudokuScoreBoard() {
        _chkButton = false;
        _genNewButton = false;
        _solButton    = false;

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

    void  setGameLevel(int level) {
        std::lock_guard<std::mutex> lck(_mtx);
        _gameLevel = level;}

    int  getGameLevel() {
        std::lock_guard<std::mutex> lck(_mtx);
        return _gameLevel;}

    void setSudokuComplete(bool complete) {
        std::lock_guard<std::mutex> lck(_mtx);
        _sudokuComplete = complete;}
    
    bool getSudokuComplete() {
        std::lock_guard<std::mutex> lck(_mtx);
        return _sudokuComplete;}

    void setUpdaterDone(bool done) {
        std::lock_guard<std::mutex> lck(_mtx);
        _updaterDone = done;}
    
    bool getUpdaterDone() {
        std::lock_guard<std::mutex> lck(_mtx);
        return _updaterDone;}

private:
    bool _chkButton;
    bool _genNewButton;
    bool _solButton;
    int  _gameLevel;
    bool _sudokuComplete;
    bool _updaterDone;
    std::mutex _mtx; 
};

#endif