#pragma once
#include "RenderObject.h"

class Disc : public RenderObject
{
public:
	Disc(float _xPos = 0, float _yPos = 0);
	~Disc();

	void SetSize(int _size);
	void Update();
	virtual	void Draw();
	void SetTargetLocation(float _x, float _y);
	bool CheckIsMoving() { return isMoving; }
	
private:
	// Values for target location of disc
	float targetX, targetY;
	// Size of this disc (used for width and checking placement)
	int size;
	// Width of visual aspect (calculated from size)
	float width;
	// Flag for checking if this disc is moving to a new tower
	bool isMoving;
};

