#pragma once
#include <assert.h>
#include "Model.h"
class Disc : public Model
{
public:
	Disc();
	~Disc();
	// Used to initialize the point vertices to build disc from
	// This must be called before Create or Draw
	void InitVertArray(int _discIndex);

	void Create(float _offsetX, float _offsetY);
	void SetTargetOffsets(float _xTarget, float _yTarget);
	// Moves disc towards its new location, returns false when no further udpates
	bool UpdatePositions();
	
	virtual void Draw() override final;
	virtual void Update() override final;

	// Used to track the new x position of this disc
	float xOffsetTargetValue;
	// Used to track the new y position of this disc
	float yOffsetTargetValue;

private:
	// Total vert count - same across all discs
	static int const discVerticesCount = 36;
	// Offset values for disc position
	float xOffset;
	float yOffset;

	// Create a quad for this tower
	// A, B, C, and D are references to the vertices array
	void DiscQuad(int _a, int _b, int _c, int _d);
	// Flag to check if arrays have been initialized
	bool bInitWasCalled;
	// Global index counter
	int currentIndex;
	// Vertices of the discs
	point4 discVertices[8];
	// Array for points of quads
	point4 discPoints[discVerticesCount];
	// Array for normals of quads
	vec3 discNormals[discVerticesCount];
};

