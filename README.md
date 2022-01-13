# CPP Nano Degree Capstone: Sudoku Game

Sudoku is a logic-based number placement puzzle. The word Sudoku is short for _Su-ji wa dokushin ni kagiru_ which means "the numbers must be single". A 9 by 9 grid partially filled with digits is a starting point. The objective is to fill the grid with the constraint that every row, column, and box (3 by 3 subgrid) must contain all of the digits from 1 to 9. The puzzle has a single solution.

<img src="Sudoku_Game.gif"/>

This project solves the sudoku using fast Backtracking (depth-first search) algorithm. The key is to place a number tentatively and find violations, backtrack if there are violations or record the number if none.

## Dependencies for Running Locally
* cmake >= 3.11
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux) 
  * Linux: make is installed by default on most Linux distros
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* SDL2_ttf >= 2.0
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  
## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SudokuGame`

## My Learnings from this project
Working from ground up on the Udacity Capstone project was a huge learning curve. I started out by exploring the Tensorflow C++ APIs and figuring out how I can showcase a Computer Vision project. I had to abandon the project as working with Tensorflow C++ APIs was extremely cumbersome and I felt modern C++ concepts were not fully utilized.

Udacity provided with a github repo of snake game to be used as starter repo. It provides cmake for SDL2 and I had to add cmake file for SDL TTF used for fonts.

  ### Game Programming

  Game Programming is a vast subject and I had not delved into that before, hence my reluctance to pick game development. It was a fun ride and I learned a lot!

  There are many approaches to writing and organizing game code. Additionally, there are huge number of libraries, platforms, tools and languages that can be used to build a professional-quality game. The libraries and tools are hard to track if you are a novice.

  > The good part is that core tenets of game programming are nearly universal and after coding up this project I feel the game loop is transferable to computer vision projects.
  > 

 ### The Game Loop Overview

 The game loop is a while loop that runs continuously in your code and has three steps:

 ![https://video.udacity-data.com/topher/2019/July/5d25488a_the-game-loop/the-game-loop.jpg](https://video.udacity-data.com/topher/2019/July/5d25488a_the-game-loop/the-game-loop.jpg)

 The Game Loop: Input, Update, Render

 - **Input**: This step handles user input from a keyboard, joystick, game controller, or other input device.
 - **Update**: This step uses the input to update the internal state of the game. The game state might include:
     - positions of characters in the game world
     - the speed, health, or inventory of characters in the game
     - how many points have been scored in the game so far
     - any other attributes or data in the game

     Each part of the game state might be updated independently of the input as well. For example, if a character is moving forward in the game with a given velocity, the update step might change the character's position without any additional input.

 - **Render**: This step takes the game state and renders the state to the screen according to fixed rules. For example, a character might be rendered with a particular image or "[sprite](https://en.wikipedia.org/wiki/Sprite_%28computer_graphics%29)", or a texture might be applied to the background of the game window.

 One major benefit of using this design pattern in a game is that each part of the game loop can be implemented separately in the code. If you want to change the appearance of your game without making major changes to how the game works, you can just update the Rendering code. Similarly, you are free to modify how the gameplay works without changing the rendering or input portion of the code at all.

Book: Game Programming Patterns [[url](https://gameprogrammingpatterns.com/game-loop.html)]

 

## Acknowledgement
1. This project used the starter game repo provided for the capstone project [Udacity Capstone Snake Game Example](https://github.com/udacity/CppND-Capstone-Snake-Game)
2. Thanks to Shing-Yan Loo for his [Pong game repo](https://github.com/yan99033/CppND-Capstone-Pong)
3. Thanks to Ryan Tabar for his [Sudoku game using SDL2 repo](https://github.com/SirFourier/Sudoku-game-using-SDL2)



## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
