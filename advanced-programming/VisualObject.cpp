#include "VisualObject.h"

VisualObject::VisualObject(float x, float y, float z) : GameObject()
{
	posX = x;
	posY = y;
	SetScale(z);
}

void VisualObject::Draw()
{
	glPushMatrix();
	glTranslatef(posX, posY, 0);
	glColor3f(0.0, 1.0, 0.0);
	glutWireCube(scaleZ);
	glPopMatrix();
}

VisualObject::~VisualObject(void)
{
}
