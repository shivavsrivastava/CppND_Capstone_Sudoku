/* Author      : Shiva Srivastava
 * Description : Renderer Class.
 *               This class renders the sudoku board and various buttons
 * 
 */
#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include "SDL.h"
#include "SDL_ttf.h"
#include "sudokucell.h"
#include "button.h"
#include "game.h"
#include "sudokuscoreboard.h"


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
                  std::shared_ptr<SudokuScoreBoard>& boardPtr)
          {_grid = grid; _buttons = buttons; _boardPtr = boardPtr;};
    void initRender();

  private:

    void loadTexture(SDL_Texture*& texture, std::string text, SDL_Color& fontColor);

    void preloadTextures();

    void createInterfaceLayout();

    void freeTextures();

    void renderFrame();

    const std::size_t                    _screenWidth;
    const std::size_t                    _screenHeight;
    const std::size_t                    _gridWidth;
    const std::size_t                    _gridHeight;
    const std::size_t                    _fontSize;
    const int                            _nTextures;
    const int                            _nRows;
    const int                            _nCols;

    // SDL stuff
    SDL_Renderer*                        _sdlRenderer;
    TTF_Font*                            _ttfFont;
    SDL_Texture*                         _textureCache[14]; 
    SDL_Window*                          _sdlWindow;
    SDL_Color                      _whiteColor={ 200, 200, 200, SDL_ALPHA_OPAQUE };
	  SDL_Color                      _greenColor={ 91, 191, 116, SDL_ALPHA_OPAQUE };
	  SDL_Color                      _redColor={ 200, 73, 46, SDL_ALPHA_OPAQUE };
    
    std::vector<std::shared_ptr<Cell>>   _grid;
    std::vector<std::shared_ptr<Button>> _buttons;
    std::shared_ptr<SudokuScoreBoard>    _boardPtr;
};

#endif
