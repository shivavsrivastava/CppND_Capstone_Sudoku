#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screenWidth,
                   const std::size_t screenHeight,
                   const std::size_t gridWidth, 
                   const std::size_t gridHeight,
                   const std::size_t fontSize)
    : _screenWidth(screenWidth),
      _screenHeight(screenHeight),
      _gridWidth(gridWidth),
      _gridHeight(gridHeight),
      _fontSize(fontSize),
      _nTextures(17),
      _nRows(9),
      _nCols(9) {

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize.\n";
		std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
	}

	// Initialize TTF
	if(TTF_Init() < 0) {
		std::cerr << "SDL_ttf could not initialize.\n";
		std::cerr << "TTF_Error: " << TTF_GetError() << "\n";
	}

	// Create Window
	_sdlWindow = SDL_CreateWindow("SUDOKU", SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED, _screenWidth,
									_screenHeight, SDL_WINDOW_SHOWN);

	if (nullptr == _sdlWindow) {
		std::cerr << "Window could not be created.\n";
		std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
	}

	// Create renderer
	_sdlRenderer = SDL_CreateRenderer(_sdlWindow, -1, SDL_RENDERER_ACCELERATED);
	if (nullptr == _sdlRenderer) {
		std::cerr << "Renderer could not be created.\n";
		std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
	}

	// Load font for test
	_ttfFont = TTF_OpenFont("../assets/octin_sports_free.ttf", _fontSize);
	if(_ttfFont == nullptr) {
		std::cerr << "TTF font could not be loaded.\n";
		std::cerr << "TTF_Error: " << TTF_GetError() << "\n";
	} 


}

Renderer::~Renderer() {
	// Disable text input
	SDL_StopTextInput();

	// Free textures
	freeTextures();

	// Destroy renderer and window
	SDL_DestroyRenderer(_sdlRenderer);
	SDL_DestroyWindow(_sdlWindow);
	_sdlRenderer = nullptr;
	_sdlWindow   = nullptr;

	// Free up the fonts
	TTF_CloseFont(_ttfFont);
	_ttfFont = nullptr;

	// Quit SDL and TTF
	SDL_Quit();
	TTF_Quit();
}

void Renderer::initRender() {

	// Preload Textures
	preloadTextures();

	// Create the layout
  	createInterfaceLayout();

	// Enable text input
	SDL_StartTextInput();


}


void Renderer::render() {
	// Clear screen prepareScene
	// Pixel color white (cream)
	SDL_SetRenderDrawColor(_sdlRenderer, 200, 200, 200, SDL_ALPHA_OPAQUE);

	// Clear screen
	SDL_RenderClear(_sdlRenderer);

    // Render the Sudoku Frame
	renderFrame();
	//std::cout << "Renderer::Render \n";

	// Update screen 
	SDL_RenderPresent(_sdlRenderer);

	// slow down the program a bit
	SDL_Delay(10);
}

void Renderer::updateWindowTitle(int fps) {
  std::string title{"SUDOKU:  FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(_sdlWindow, title.c_str());
}


void Renderer::loadTexture(SDL_Texture*& texture, std::string text, SDL_Color& fontColor)
{
	// Create text surface
	SDL_Surface* surface = TTF_RenderText_Solid(_ttfFont, text.c_str(), fontColor);
	if (surface == nullptr)
	{
		std::cout << "Could not create TTF SDL_Surface! Error: " << TTF_GetError() << "\n";
	}
	else
	{
		// Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(_sdlRenderer, surface);
		if (texture == nullptr)
		{
			std::cout << "Could not create texture from surface! Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(surface);
	}
}

void Renderer::preloadTextures() {
	// Choose color of text
	SDL_Color textColor = { 200, 200, 200, SDL_ALPHA_OPAQUE }; // white

	// Load texture for empty space
	loadTexture(_textureCache[0], " ", textColor);

	// Load textures for numbers from 1 to 9
	for (int num = 1; num < 10; num++)
	{
		std::string temp = std::to_string(num);
		loadTexture(_textureCache[num], temp.c_str(), textColor);
	}

	// Load texture for "Check", "Solve", and "New" buttons
	loadTexture(_textureCache[10], "Level: Beginner", textColor);
    loadTexture(_textureCache[11], "Timer", textColor);
	loadTexture(_textureCache[12], "Check", textColor);
    loadTexture(_textureCache[13], "Solution", textColor); // repeating the same color for now
	loadTexture(_textureCache[14], "New", textColor);
	loadTexture(_textureCache[15], "Wrong!", textColor);
	loadTexture(_textureCache[16], "Right!", textColor);
}

void Renderer::freeTextures()
{
	for (int i = 0; i < _nTextures; i++)
	{
		// Free texture if it exists
		if (_textureCache[i] != nullptr)
		{
			SDL_DestroyTexture(_textureCache[i]);
			_textureCache[i] = nullptr;
		}
	}
}

/* This is how I want the Sudoku to be laid out
   ===========================================================
   || Level:Beginner               StopWatch: 00:00:00      ||
   ===========================================================
   ||  2  |  5  |     ||  7  |  1  |     ||  6  |     |  8  ||
   ||-------------------------------------------------------||
   ||  9  |     |  3  ||  8  |     |     ||     |  7  |  2  ||
   ||-------------------------------------------------------||
   ||     |  8  |  7  ||     |     |  4  ||     |     |  1  ||
   ||=======================================================||
   ||     |  6  |  2  ||  4  |     |  3  ||  5  |     |     ||
   ||-------------------------------------------------------||
   ||     |  7  |  9  ||     |  6  |     ||     |  2  |  3  ||
   ||-------------------------------------------------------||
   ||     |     |     ||  9  |     |  1  ||     |  4  |     ||
   ||=======================================================||
   ||     |     |  5  ||     |  9  |     ||     |     |     ||
   ||-------------------------------------------------------||
   ||  7  |     |  6  ||     |  8  |     ||     |  9  |  4  ||
   ||-------------------------------------------------------||
   ||  8  |     |  1  ||     |  4  |     ||     |     |     ||
   ||=======================================================||
   ||      CHECK      ||    SOLUTION     ||       NEW       ||
   ||=======================================================||
*/

void Renderer::createInterfaceLayout()
{
	// Define thick and thin borders
	const int thinBorder    = 2;
	const int thickBorder   = thinBorder + 6;
  	const int thickerBorder = thinBorder + 6;
  	const int finalRows     = _nRows + 2;
	  int     nOtherButtons = 3;

	/*-----------Display Stopwatch and Level of Game----------------------------*/
	// Game Level and Stopwatch are unclickable buttons
	// gridwidth and screen width are same
	int buttonStartRow = 0;
	int buttonWidth = _gridWidth - 3 * thickerBorder; 
	// screenHeight = buttonHeight + 6 * thinBorder + 2 * thickBorder + 4*thickerBorder
	int buttonHeight = (_screenHeight - 6 * thinBorder - 2 * thickBorder - 4 * thickerBorder) / finalRows;

	buttonStartRow += thickerBorder;
	int buttonStartCol = 0;
	buttonStartCol += thickerBorder;
  	// Set the level button
  	SDL_Rect buttonRect1 = { buttonStartCol, buttonStartRow, buttonWidth/2, buttonHeight };
	_buttons.at(0)->setButtonRect(buttonRect1);
	_buttons.at(0)->setTexture(_textureCache[10]);
	

	buttonStartCol += buttonWidth/2 + thickBorder;

	// Set button position and dimensions
	SDL_Rect buttonRect2 = { buttonStartCol, buttonStartRow, buttonWidth/2, buttonHeight };
	_buttons.at(1)->setButtonRect(buttonRect2);
	_buttons.at(1)->setTexture(_textureCache[11]);

	/*-------------Create buttons for each cell-------------------------------------*/
	// Define cell button dimensions
	// gridWidth = 6 * thinBorder + 4 * thickBorder + 9 * buttonWidth
	buttonWidth = (_gridWidth - 6 * thinBorder - 2 * thickBorder - 2 * thickerBorder) / _nCols;

	// Carry on from previous starting row
	buttonStartRow += buttonHeight;

	// Set cell button position and dimensions
	for (int gridRow = 0; gridRow < _nRows; gridRow++)
	{
		// Add to starting row
		if (gridRow == 0) buttonStartRow += thickerBorder;
		else if (gridRow % 3 == 0) buttonStartRow += buttonHeight + thickBorder;
		else buttonStartRow += buttonHeight + thinBorder;

		// Reset starting column
		int buttonStartCol = 0;

		for (int gridCol = 0; gridCol < _nCols; gridCol++)
		{
			// Add to starting column
			if (gridCol == 0) buttonStartCol += thickerBorder;
			else if (gridCol % 3 == 0) buttonStartCol += buttonWidth + thickBorder;
			else buttonStartCol += buttonWidth + thinBorder;

			// Set button position and dimensions
			SDL_Rect buttonRect = { buttonStartCol, buttonStartRow, buttonWidth, buttonHeight };
			int index = _nRows * gridRow + gridCol;
			_grid.at(index)->setButtonRect(buttonRect);
			if(_grid[index]->getNumber() ==0)
				_grid.at(index)->setTexture(_textureCache[0]);
			else {
				int num = _grid[index]->getNumber();
				_grid.at(index)->setTexture(_textureCache[num]);
			}
		}
	}

	/*-----------Create check, and new buttons--------------------------------*/
	_buttons.at(3)->setTexture(_textureCache[13]);
	_buttons.at(2)->setTexture(_textureCache[12]);
	_buttons.at(4)->setTexture(_textureCache[14]);

	// Redefine button width
	// gridWidth = 4 * thickBorder + 9 * numberOfOtherButtons (rearrange this equation)
	buttonWidth = (_gridWidth - 4 * thickBorder) / nOtherButtons;

	// Carry on from previous starting row
	buttonStartRow += buttonHeight + thickBorder;

	// Reset starting column
	int borderWidthTotal = 0;

	// Set check, solve, and new buttons (last row)
	for (int button = 0; button < nOtherButtons; button++) // colBlock is every 3 columns of cells
	{
		// Add border width to total
		if (button == 0) borderWidthTotal += thickBorder;
		else borderWidthTotal += thickBorder;
		int buttonStartCol = button * buttonWidth + borderWidthTotal;

		// Set button position and dimensions
		SDL_Rect buttonRect = { buttonStartCol, buttonStartRow, buttonWidth, buttonHeight };
		_buttons.at(2+button)->setButtonRect(buttonRect);
	}

}

void Renderer::renderFrame() {
	// Choose color of text
	SDL_Color textColor = { 200, 200, 200, SDL_ALPHA_OPAQUE }; // white
	SDL_Color solColor = { 91, 191, 116, SDL_ALPHA_OPAQUE }; // green
	SDL_Color redColor = { 200, 73, 46, SDL_ALPHA_OPAQUE }; // red


	// When solution button is clicked, then show completed Sudoku, with solution in green
	if(_boardPtr->getSolButton()) { // when solution button clicked
		for (int index = 0; index < _grid.size(); index++) {
			// Render button
			_grid[index]->renderButton(_sdlRenderer);
			int sol = _grid[index]->getSolution();
			std::string solString = std::to_string(sol);
			if(_grid[index]->isEditable()) {
				loadTexture(_textureCache[sol], solString.c_str(), solColor);
			} else {
				loadTexture(_textureCache[sol], solString.c_str(), textColor);
			}
			_grid[index]->setTexture(_textureCache[sol]);
			// Re-center since diffrerent numbers have different sized textures
			_grid[index]->centerTextureRect();

			// Render texture
			_grid[index]->renderTexture(_sdlRenderer);
		}

	} 
	// When check button is clicked, then numbers entered in the sudoku are wrong or right
	else if(_boardPtr->getCheckButton()) { 
		for (int index = 0; index < _grid.size(); index++) {
			// Render button
			_grid[index]->renderButton(_sdlRenderer);
			int num = _grid[index]->getNumber();
			std::string numString = std::to_string(num);
			if(_grid[index]->isEditable() && num!=0) {
				if(_grid[index]->isCorrect())
					loadTexture(_textureCache[num], numString.c_str(), solColor);
				else 
					loadTexture(_textureCache[num], numString.c_str(), redColor);
			} else {
				if(num!=0)
					loadTexture(_textureCache[num], numString.c_str(), textColor);
				else {
					numString = " ";
					loadTexture(_textureCache[num], numString.c_str(), textColor);
				}
			}
			_grid[index]->setTexture(_textureCache[num]);
			// Re-center since diffrerent numbers have different sized textures
			_grid[index]->centerTextureRect();

			// Render texture
			_grid[index]->renderTexture(_sdlRenderer);
		}
	}
	// normal case 
	else {
		for (int index = 0; index < _grid.size(); index++) {
			// Render button
			_grid[index]->renderButton(_sdlRenderer);
			int num = _grid[index]->getNumber();
			std::string numString = " ";
			if(num!=0)
				numString = std::to_string(num);
			loadTexture(_textureCache[num], numString.c_str(), textColor);
			_grid[index]->setTexture(_textureCache[num]);
			// Re-center since diffrerent numbers have different sized textures
			_grid[index]->centerTextureRect();

			// Render texture
			_grid[index]->renderTexture(_sdlRenderer);
		}
	}

	// Render other buttons
	int baseIndex = 10; // start of buttons
	for (int index = 0; index < _buttons.size(); index++)
	{
		// Render Level Button
		if(index==0) {
			int level = _boardPtr->getGameLevel();
			int cIndex = baseIndex + index;
			if(level <= 3)
				loadTexture(_textureCache[cIndex], "Level: Beginner", textColor);
			else if(level > 3 && level <=6)
				loadTexture(_textureCache[cIndex], "Level: Advanced", textColor);
			else
				loadTexture(_textureCache[cIndex], "Level: Expert", textColor);
			_buttons[index]->setTexture(_textureCache[cIndex]);
		}

		// Render button
		_buttons[index]->renderButton(_sdlRenderer);

		// Re-center since diffrerent numbers have different sized textures
		_buttons[index]->centerTextureRect();

		// Render texture
		_buttons[index]->renderTexture(_sdlRenderer);
	}
}