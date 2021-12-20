#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include "SDL.h"
#include "SDL_ttf.h"
#include "sudokucell.h"
#include "button.h"
#include "game.h"
#include "sudokuboard.h"


class Game;

class Renderer {
  public:
  Renderer(const std::size_t screenWidth, const std::size_t screenHeight,
           const std::size_t gridWidth, const std::size_t gridHeight,
           const std::size_t fontSize);
  ~Renderer();

  void render();
  void updateWindowTitle(int fps);
  void setBoard(std::vector<std::shared_ptr<Cell>>& grid, 
              std::vector<std::shared_ptr<Button>>& buttons,
                std::shared_ptr<SudokuBoard>& boardPtr)
        {_grid = grid; _buttons = buttons; _boardPtr = boardPtr;};
  void initRender();

 private:

  void loadTexture(SDL_Texture*& texture, std::string text, SDL_Color& fontColor);

  void preloadTextures();

  void createInterfaceLayout();

  void freeTextures();

  void renderFrame();


  SDL_Window*   _sdlWindow;
  SDL_Renderer* _sdlRenderer;
  TTF_Font*     _ttfFont;
  SDL_Texture*  _textureCache[17]; 
  

  const std::size_t _screenWidth;
  const std::size_t _screenHeight;
  const std::size_t _gridWidth;
  const std::size_t _gridHeight;
  const std::size_t _fontSize;
  const int         _nTextures;
  const int         _nRows;
  const int         _nCols;

  std::vector<std::shared_ptr<Cell>>   _grid;
  std::vector<std::shared_ptr<Button>> _buttons;
  std::shared_ptr<SudokuBoard>         _boardPtr;
};

#endif
