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
	FreeTextures();

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

void Renderer::InitRender() {

	// Preload Textures
	PreloadTextures();

	// Create the layout
  	CreateInterfaceLayout();

	// Enable text input
	SDL_StartTextInput();


}


void Renderer::Render() {
	// Clear screen prepareScene
	// Pixel color white (cream)
	SDL_SetRenderDrawColor(_sdlRenderer, 200, 200, 200, SDL_ALPHA_OPAQUE);

	// Clear screen
	SDL_RenderClear(_sdlRenderer);

    // Render the Sudoku Frame
	RenderFrame();
	//std::cout << "Renderer::Render \n";

	// Update screen 
	SDL_RenderPresent(_sdlRenderer);

	// slow down the program a bit
	SDL_Delay(10);
}

void Renderer::UpdateWindowTitle(int fps) {
  std::string title{"SUDOKU:  FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(_sdlWindow, title.c_str());
}


void Renderer::LoadTexture(SDL_Texture*& texture, std::string text, SDL_Color& fontColor)
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

void Renderer::PreloadTextures() {
	// Choose color of text
	SDL_Color textColor = { 200, 200, 200, SDL_ALPHA_OPAQUE }; // white

	// Load texture for empty space
	LoadTexture(_textureCache[0], " ", textColor);

	// Load textures for numbers from 1 to 9
	for (int num = 1; num < 10; num++)
	{
		std::string temp = std::to_string(num);
		LoadTexture(_textureCache[num], temp.c_str(), textColor);
	}

	// Load texture for "Check", "Solve", and "New" buttons
	LoadTexture(_textureCache[10], "Level: Beginner", textColor);
    LoadTexture(_textureCache[11], "Timer", textColor);
	LoadTexture(_textureCache[12], "Check", textColor);
    LoadTexture(_textureCache[13], "Solution", textColor); // repeating the same color for now
	LoadTexture(_textureCache[14], "New", textColor);
	LoadTexture(_textureCache[15], "Wrong!", textColor);
	LoadTexture(_textureCache[16], "Right!", textColor);
}

void Renderer::FreeTextures()
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

void Renderer::CreateInterfaceLayout()
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
	_buttons.at(0)->SetButtonRect(buttonRect1);
	_buttons.at(0)->SetTexture(_textureCache[10]);
	

	buttonStartCol += buttonWidth/2 + thickBorder;

	// Set button position and dimensions
	SDL_Rect buttonRect2 = { buttonStartCol, buttonStartRow, buttonWidth/2, buttonHeight };
	_buttons.at(1)->SetButtonRect(buttonRect2);
	_buttons.at(1)->SetTexture(_textureCache[11]);

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
			_grid.at(index)->SetButtonRect(buttonRect);
			if(_grid[index]->GetNumber() ==0)
				_grid.at(index)->SetTexture(_textureCache[0]);
			else {
				int num = _grid[index]->GetNumber();
				_grid.at(index)->SetTexture(_textureCache[num]);
			}
		}
	}

	/*-----------Create check, and new buttons--------------------------------*/
	_buttons.at(2)->SetTexture(_textureCache[12]);
	_buttons.at(3)->SetTexture(_textureCache[13]);
	_buttons.at(4)->SetTexture(_textureCache[14]);

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
		_buttons.at(2+button)->SetButtonRect(buttonRect);
	}

}

void Renderer::RenderFrame() {
	// Render the sudoku cells
	for (int index = 0; index < _grid.size(); index++)
	{
		// Render button
		_grid[index]->RenderButton(_sdlRenderer);

		int num = _grid[index]->GetNumber();
		_grid[index]->SetTexture(_textureCache[num]);
		// Re-center since diffrerent numbers have different sized textures
		_grid[index]->CenterTextureRect();

		// Render texture
		_grid[index]->RenderTexture(_sdlRenderer);
	}

	// Render other buttons
	for (int index = 0; index < _buttons.size(); index++)
	{
		// Render button
		_buttons[index]->RenderButton(_sdlRenderer);

		// Re-center since diffrerent numbers have different sized textures
		_buttons[index]->CenterTextureRect();

		// Render texture
		_buttons[index]->RenderTexture(_sdlRenderer);
	}
}