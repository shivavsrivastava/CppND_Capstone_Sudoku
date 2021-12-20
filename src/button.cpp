#include "button.h"

Button::Button()
	: _currentState(ButtonState::BUTTON_MOUSE_OUT),
	  _texture(nullptr),
	  _buttonRect({ 0, 0, 0, 0 }),
	  _textureRect({0, 0, 0, 0}),
	  _mouseOutC({ 81, 78, 78, SDL_ALPHA_OPAQUE }), // gray
	  _mouseOverC({ 95, 89, 191, SDL_ALPHA_OPAQUE }),//blue
	  _mouseDownC({ 91, 191, 116, SDL_ALPHA_OPAQUE }), // green
	  _mouseUpC({ 95, 89, 191, SDL_ALPHA_OPAQUE }), // blue
	  _selected(false) {}

Button::Button(int id)
	: _currentState(ButtonState::BUTTON_MOUSE_OUT),
	  _texture(nullptr),
	  _buttonRect({ 0, 0, 0, 0 }),
	  _textureRect({0, 0, 0, 0}),
	  _mouseOutC({ 81, 78, 78, SDL_ALPHA_OPAQUE }), // gray
	  _mouseOverC({ 95, 89, 191, SDL_ALPHA_OPAQUE }),//blue
	  _mouseDownC({ 91, 191, 116, SDL_ALPHA_OPAQUE }), // green
	  _mouseUpC({ 95, 89, 191, SDL_ALPHA_OPAQUE }), // blue
	  _selected(false),
	  _id(id) {}


void Button::setTexture(SDL_Texture* texture)
{
	_texture = texture;
}

void Button::setButtonRect(const SDL_Rect& rect)
{
	_buttonRect = rect;
}

void Button::setTextureRect(const SDL_Rect& rect)
{
	_textureRect = rect;
}

void Button::centerTextureRect()
{
	int textureWidth;
	int textureHeight;
	SDL_QueryTexture(_texture, NULL, NULL, &textureWidth, &textureHeight);

	const int textureStartRow = _buttonRect.y + 0.5 * (_buttonRect.h - textureHeight);
	const int textureStartCol = _buttonRect.x + 0.5 * (_buttonRect.w - textureWidth);

	_textureRect = { textureStartCol, textureStartRow, textureWidth, textureHeight };
}

void Button::setSelected(const bool selected)
{
	_selected = selected;
}

bool Button::isMouseInside(const int x, const int y)
{

	// Check if mouse is in button
	bool inside = true;

	// Mouse is left of the button
	if (x < _buttonRect.x)
	{
		inside = false;
	}
	// Mouse is right of the button
	else if (x > _buttonRect.x + _buttonRect.w)
	{
		inside = false;
	}
	// Mouse above the button
	else if (y < _buttonRect.y)
	{
		inside = false;
	}
	// Mouse below the button
	else if (y > _buttonRect.y + _buttonRect.h)
	{
		inside = false;
	}

	return inside;
}

ButtonState Button::getMouseEvent(const SDL_Event* event)
{
	//If mouse event happened
	if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP)
	{
		// Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		// Mouse is outside button
		if (!isMouseInside(x, y))
		{
			_currentState = ButtonState::BUTTON_MOUSE_OUT;
		}
		// Mouse is inside button
		else
		{
			// Set mouse over State
			switch (event->type)
			{
			case SDL_MOUSEMOTION:
				_currentState = ButtonState::BUTTON_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				_currentState = ButtonState::BUTTON_MOUSE_DOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				_currentState = ButtonState::BUTTON_MOUSE_UP;
				break;

			}
		}
	}
	return _currentState;
}

void Button::setMouseDownColor(const SDL_Color& color)
{
	_mouseDownC = color;
}

void Button::renderButton(SDL_Renderer* renderer)
{
	// If current button is selected
	if (_selected)
	{
		SDL_SetRenderDrawColor(renderer, _mouseDownC.r, _mouseDownC.g, _mouseDownC.b, _mouseDownC.a);
	}
	else
	{
		// Show current button State
		switch (_currentState)
		{
		case ButtonState::BUTTON_MOUSE_OUT:
			SDL_SetRenderDrawColor(renderer, _mouseOutC.r, _mouseOutC.g, _mouseOutC.b, _mouseOutC.a);
			break;
		case ButtonState::BUTTON_MOUSE_OVER_MOTION:
			SDL_SetRenderDrawColor(renderer, _mouseOverC.r, _mouseOverC.g, _mouseOverC.b, _mouseOverC.a);
			break;
		case ButtonState::BUTTON_MOUSE_DOWN:
			SDL_SetRenderDrawColor(renderer, _mouseDownC.r, _mouseDownC.g, _mouseDownC.b, _mouseDownC.a);
			break;
		case ButtonState::BUTTON_MOUSE_UP:
			SDL_SetRenderDrawColor(renderer, _mouseUpC.r, _mouseUpC.g, _mouseUpC.b, _mouseUpC.a);
			break;

		}
	}

	SDL_RenderFillRect(renderer, &_buttonRect);
}

void Button::renderTexture(SDL_Renderer* renderer)
{
	// Set rendering space
	SDL_RenderCopy(renderer, _texture, nullptr, &_textureRect);
}

