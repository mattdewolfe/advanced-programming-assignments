#ifndef VISUAL_OBJECT_H
#define VISUAL_OBJECT_H

#include "GameObject.h"
#include "openGL\glut.h"

class VisualObject : public GameObject
{
public:
	VisualObject(float x, float y, float z);
	virtual ~VisualObject(void);
	// return object type (as char)
	char* GetType() const { return "VisualObject"; }
	// visuals
	void Draw();
	float GetX() { return posX; }
	float GetY() { return posY; }
	float SetX(float _x) { posX = _x; }
	float SetY(float _y) { posY = _y; }
	
protected:
	float posX, posY;

};

#endif