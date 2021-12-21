#include "sudokucell.h"

Cell::Cell()
	: _editable(false),
	  _number(0), 
	  _solution(0) {}

Cell::Cell(int id)
	: _editable(false),
	  _number(0), 
	  _solution(0)
{
	_id = id;
}

void Cell::setNumber(const int number)
{
	std::lock_guard<std::mutex> lck(_bmtx);
	if (number <= 9 && number>= 1)
	{
		_number = number;
	}
	else
	{
		_number = 0;
	}
}

int Cell::getNumber()
{
	std::lock_guard<std::mutex> lck(_bmtx);
	return _number;
}

void Cell::setSolution(const int solution)
{
	std::lock_guard<std::mutex> lck(_bmtx);
	if (solution <= 9 && solution>= 1)
	{
		_solution = solution;
	}
	else
	{
		_solution = 0;
	}
}

void Cell::setEditable(const bool editable)
{
	std::lock_guard<std::mutex> lck(_bmtx);
	_editable = editable;
	if (_editable)
	{
		_mouseOutC = { 102, 103, 171, SDL_ALPHA_OPAQUE }; // pantone very peri color of 2022
		_mouseOverC = { 95, 89, 191, SDL_ALPHA_OPAQUE }; // blue
		_mouseOverC = { 91, 191, 116, SDL_ALPHA_OPAQUE }; // green
		_mouseUpC = { 95, 89, 191, SDL_ALPHA_OPAQUE }; // blue
	}
	else
	{
		_mouseOutC = { 82, 83, 141, SDL_ALPHA_OPAQUE }; // dark peri
		_mouseOverC = { 82, 83, 141, SDL_ALPHA_OPAQUE }; // dark peri
		_mouseOverC = { 82, 83, 141, SDL_ALPHA_OPAQUE }; // dark peri
		_mouseUpC = { 82, 83, 141, SDL_ALPHA_OPAQUE }; // dark peri
	}
}
bool Cell::isEditable()
{
	std::lock_guard<std::mutex> lck(_bmtx);
	return _editable;
}

void Cell::handleKeyboardEvent(const SDL_Event* event)
{
	// Handle backspace
	if (event->key.keysym.sym == SDLK_BACKSPACE && _number != 0)
	{
		std::lock_guard<std::mutex> lck(_bmtx);
		// Empty number
		_number = 0;
	}
	// Handle text input
	else if (event->type == SDL_TEXTINPUT)
	{
		// Check if number lies between 1 to 9, as those are valid sudoku numbers
		std::cout << "Cell::HandleKeyboardEvent -- text is" << event->text.text << "\n";
		_number = atoi(event->text.text);
		this->setNumber(_number);
	}
}


bool Cell::isCorrect()
{
	std::lock_guard<std::mutex> lck(_bmtx);
	return (_number == _solution);
}

