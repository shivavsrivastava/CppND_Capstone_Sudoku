#include <iostream>
#include <fstream>
#include "SDL.h"
#include "SDL_ttf.h"
#include "game.h"


void Game::play(Controller& controller, 
                Renderer& renderer,
                Updater& updater,
                std::size_t targetFrameDuration) {
    Uint32 titleTimestamp = SDL_GetTicks();
    Uint32 frameStart;
    Uint32 frameEnd;
    Uint32 frameDuration;
    int    frameCount = 0;
    SDL_Event e;
  

    // Start controller thread
    _threads.emplace_back(std::thread(&Controller::handleInput, std::ref(controller)));

    // Start updater thread - function run
    _threads.emplace_back(std::thread(&Updater::run, std::ref(updater)));
    
    // Initialize render frame
    renderer.initRender();

    SDL_StartTextInput();
    
    // main game loop
    while (!controller.readyToStop()) {
        frameStart = SDL_GetTicks();

        // Call the render function from this loop
        renderer.render();

        frameEnd = SDL_GetTicks();

        // Keep track of how long each loop through the render cycle takes
        frameCount++;
        frameDuration = frameEnd - frameStart;


        // After every second, update the window title.
        if (frameEnd - titleTimestamp >= 1000) {
            renderer.updateWindowTitle(frameCount);
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
    controller.stop();
    updater.stop();

    // Join all threads here
    for(auto &t : _threads)
        t.join();
}
