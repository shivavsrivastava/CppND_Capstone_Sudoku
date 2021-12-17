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
	int GetID() {
		return _id;}
	void SetID(int id) {
		_id = id;}

	// Set Texture
	void SetTexture(SDL_Texture* texture);

	// Set button and texture SDL_Rect
	void SetButtonRect(const SDL_Rect& rect);
	void SetTextureRect(const SDL_Rect& rect);

	// Center texture onto button
	void CenterTextureRect();

	// Set selected
	void SetSelected(const bool selected);

	// Check if mouse is inside button
	bool IsMouseInside(const int x, const int y);
	
	// Get mouse event
	ButtonState GetMouseEvent(const SDL_Event* event);

	// Set mouse down color
	void SetMouseDownColor(const SDL_Color& color);

	// Render button and texture
	void RenderButton(SDL_Renderer* renderer);
	void RenderTexture(SDL_Renderer* renderer);

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
