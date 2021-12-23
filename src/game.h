/* Author      : Shiva Srivastava
 * Description : Game Class.
 *               This class in the main instantiates the required game loop components:
 *                  - input  (Controller)
 *                  - update (Updater)
 *                  - render (Renderer)
 *               Sets Controller::handleInput and Updater::run functions in separate threads.
 *               Overall responsible for threads execution.
 * 
 * Credits     : Thanks to the starter code provided by Udacity
 * (https://github.com/udacity/CppND-Capstone-Snake-Game)
 */
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
#include "updater.h"


class Controller; // forward declaration
class Renderer; // forward declaration
class Updater;


class Game {
 public:
  Game(){};
  void play(Controller& controller, 
            Renderer& renderer,
            Updater& updater,
            std::size_t targetFrameDuration);

 private:
  int                      _gridW;
  int                      _gridH;
  std::vector<std::thread> _threads;

};

#endif
