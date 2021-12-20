#ifndef BUTTON_H
#define BUTTON_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <mutex>

enum class ButtonState
{
	BUTTON_MOUSE_OUT = 0,
	BUTTON_MOUSE_OVER_MOTION = 1,
	BUTTON_MOUSE_DOWN = 2,
	BUTTON_MOUSE_UP = 3,
};

class Button
{


public:
	// Constructor
	Button();
	Button(int id);
	

	// getter and setter
	int getId() {
		return _id;}
	void setId(int id) {
		_id = id;}

	// Set Texture
	void setTexture(SDL_Texture* texture);

	// Set button and texture SDL_Rect
	void setButtonRect(const SDL_Rect& rect);
	void setTextureRect(const SDL_Rect& rect);

	// Center texture onto button
	void centerTextureRect();

	// Set selected
	void setSelected(const bool selected);

	// Check if mouse is inside button
	bool isMouseInside(const int x, const int y);
	
	// Get mouse event
	ButtonState getMouseEvent(const SDL_Event* event);

	// Set mouse down color
	void setMouseDownColor(const SDL_Color& color);

	// Render button and texture
	void renderButton(SDL_Renderer* renderer);
	void renderTexture(SDL_Renderer* renderer);

private:
	// Current button state
	ButtonState _currentState;

	// Define texture to display
	SDL_Texture* _texture;

	// Top left position and dimensions of button and texture
	SDL_Rect _buttonRect;
	SDL_Rect _textureRect;

	// Has the button been selected by the mouse
	bool _selected;


protected:
	// Define button colors
	SDL_Color _mouseOutC;
	SDL_Color _mouseOverC;
	SDL_Color _mouseDownC;
	SDL_Color _mouseUpC;
	int       _id;
	static std::mutex _mtx; // mutex for protecting cout
};

#endif
