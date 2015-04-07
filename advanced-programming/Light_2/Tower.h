#pragma once
#include "Model.h"
class Tower : public Model
{
public:
	Tower();
	~Tower();

	void Create(float _offset);
	virtual void Draw() override final;
	virtual void Update() override;
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
	// Create a quad for this tower
	// A, B, C, and D are references to the vertices array
	void TowerQuad(int _a, int _b, int _c, int _d);

	// Global index counter
	int currentIndex;
	static int const towerVerticesCount = 36;

	// Array for points of quads
	point4 towerPoints[towerVerticesCount];
	// Array for normals of quads
	vec3 towerNormals[towerVerticesCount];

	// Store index of the top disc on this tower
	int topDiscIndex;
	// Stores the index of discs located on this tower
	int discs[6];

};

