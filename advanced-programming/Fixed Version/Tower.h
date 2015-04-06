#pragma once
#include "RenderObject.h"

class Tower : public RenderObject
{
public:
	Tower(float _xPos = 0, float _yPos = 0);
	~Tower();

	virtual void Draw();

	// Attempt to place the selected disc on this tower
	bool PlaceDiscOnTower(int _discNumber);
	// Removes the top disc from this tower
	void RemoveTopDisc();
	// Push the starting discs to this tower
	void FillDiscArrayToIndex(int _index);
	// Printout locations of discs for debugging
	void PrintTowerLocations();
	// Remove all discs from this tower
	void ClearDiscs();
	// Get the value of the topmost disc on this tower
	int GetTopDisc() { return discs[topDiscIndex]; }
	// Get number of discs on this tower
	int GetDiscCount() { return topDiscIndex + 1; }

private:

	// Store index of the top disc on this tower
	int topDiscIndex;
	// Stores the index of discs located on this tower
	int discs[6];

};

