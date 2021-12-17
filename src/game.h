#ifndef GAME_H
#define GAME_H

#include <random>
#include <thread>
#include <memory>
#include <mutex>


#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "button.h"
#include "sudokucell.h"
#include "gameengine.h"


class Controller; // forward declaration
class Renderer; // forward declaration
class GameEngine;


class Game {
 public:
  Game(std::size_t gridW, std::size_t gridH);
  void Play(Controller& controller, 
            Renderer& renderer,
            GameEngine& engine,
            std::size_t targetFrameDuration);

 private:
  int    _gridW;
  int    _gridH;
  std::vector<std::thread> _threads;

};

#endif
