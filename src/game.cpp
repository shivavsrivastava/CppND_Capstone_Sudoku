#include <iostream>
#include <fstream>
#include <chrono>
#include "SDL.h"
#include "SDL_ttf.h"
#include "game.h"

Game::Game(std::size_t gridW, std::size_t gridH)
     : _gridW(static_cast<int>(gridW)),
       _gridH(static_cast<int>(gridH)) {}

void Game::Play(Controller& controller, 
                Renderer& renderer,
                Updater& updater,
                std::size_t targetFrameDuration) {
  Uint32 titleTimestamp = SDL_GetTicks();
  Uint32 frameStart;
  Uint32 frameEnd;
  Uint32 frameDuration;
  int frameCount = 0;
  SDL_Event e;

  // Start controller thread
  
  _threads.emplace_back(std::thread(&Controller::HandleInput, std::ref(controller)));

  // Start game engine thread - function Run has one input
  _threads.emplace_back(std::thread(&Updater::Run, std::ref(updater)));
  
  // Initialize render frame
  renderer.InitRender();

  SDL_StartTextInput();
  
  // main game loop
  while (!controller.ReadyToStop()) {
    frameStart = SDL_GetTicks();
  
    renderer.Render();

    frameEnd = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frameCount++;
    frameDuration = frameEnd - frameStart;

    // After every second, update the window title.
    if (frameEnd - titleTimestamp >= 1000) {
      renderer.UpdateWindowTitle(frameCount);
      frameCount = 0;
      titleTimestamp = frameEnd;
    }

    // If the time for this frame is too small (i.e. frameDuration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frameDuration < targetFrameDuration) {
      SDL_Delay(targetFrameDuration - frameDuration);
    }
  }

  // Stop the threads
  controller.Stop();
  updater.Stop();

  // Join all threads here
  for(auto &t : _threads)
    t.join();
}
