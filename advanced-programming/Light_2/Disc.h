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
	virtual void Draw() override final;
	virtual void Update() override;

private:
	// Total vert count - same across all discs
	static int const discVerticesCount = 36;

	// Create a quad for this tower
	// A, B, C, and D are references to the vertices array
	void DiscQuad(int _a, int _b, int _c, int _d, float _offsetX, float _offsetZ);
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

