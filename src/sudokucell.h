/* Author      : Shiva Srivastava
 * Description : Cell Class is extended from Button class. 
 *               Contains functions that help in editing the sudoku cell.
 *               handleKeyboardEvent handles the numbers written.
 * 
 */
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
	void setNumber(const int number);
	int  getNumber();

	// Set solution
	void setSolution(const int solution);
	int  getSolution();

	// Set and get editability (setting editability changes color mapping for button)
	void setEditable(const bool editable);
	bool isEditable();

	// Handle events
	void handleKeyboardEvent(const SDL_Event* event);

	// Compare number with solution
	bool isCorrect();
private:
	
	bool _editable; // Define editiability (can type number in cell)
	int  _number;   // number in the cell currenlty displayed
	int  _solution; // solution 

};

#endif