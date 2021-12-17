
#ifndef CELL_H
#define CELL_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

#include "button.h"

class Cell : public Button
{
public:
	// Constructor
	Cell();
	Cell(int id);

	// Set and get number
	void SetNumber(const int number);
	int GetNumber();

	// Set solution
	void SetSolution(const int solution);

	// Set and get editability (setting editability changes color mapping for button)
	void SetEditable(const bool editable);
	bool IsEditable();

	// Handle events
	void HandleKeyboardEvent(const SDL_Event* event);

	// Compare number with solution
	bool IsCorrect();
private:
	// Define editiability (can type number in cell)
	bool _editable;

	// Define number currently displayed and the actual solution
	int _number;
	int _solution;

};

#endif