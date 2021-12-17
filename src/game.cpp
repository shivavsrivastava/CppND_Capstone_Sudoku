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
                GameEngine& engine,
                std::size_t targetFrameDuration) {
  Uint32 titleTimestamp = SDL_GetTicks();
  Uint32 frameStart;
  Uint32 frameEnd;
  Uint32 frameDuration;
  int frameCount = 0;
  bool running = true;
  bool init = true;
  SDL_Event e;

  // Start controller thread
  
  _threads.emplace_back(std::thread(&Controller::HandleInput, std::ref(controller)));

  // Start game engine thread - function Run has one input
  _threads.emplace_back(std::thread(&GameEngine::Run, std::ref(engine)));
  
  // Initialize render frame
  renderer.InitRender();

  SDL_StartTextInput();
  
  // main game loop
  while (running) {
    frameStart = SDL_GetTicks();

    // Check if an exit/interrupt signal is received
    //controller.ReadyToPoll();
    SDL_PollEvent(&e);
    if (e.type == SDL_QUIT || (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)) 
      break;

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
  engine.Stop();

  // Join all threads here
  for(auto &t : _threads)
    t.join();
}
