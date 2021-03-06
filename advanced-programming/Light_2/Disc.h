#pragma once
#include <assert.h>
#include "Model.h"
class Disc : public Model
{
public:
	Disc(int _discSize);
	~Disc();
	// Used to initialize the point vertices to build disc from
	// This must be called before Create or Draw
	void InitVertArray(int _discIndex);

	void Create(float _offsetX, float _offsetY, vec4 _color);
	// Set a new target position
	void SetTargetOffsets(float _xTarget, float _yTarget);
	// Moves disc towards its new location, returns false when no further udpates
	bool UpdatePositions();
	void ResetPosition(float _xTarget, float _yTarget);
	virtual void Draw(GLuint _modelView, mat4 _modelMat) override final;
	virtual void Update() override final;

private:
	// Total vert count - same across all discs
	static int const discVerticesCount = 36;
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
	// Size of this disc relative to those on the tower. 0 is largest
	int size;
	// Flag for when this disc is being moved
	bool isMoving;
};

