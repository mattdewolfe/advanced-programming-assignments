#include "Tower.h"

Tower::Tower(float _xPos, float _yPos) : RenderObject(_xPos, _yPos),
topDiscIndex(-1)
{
	color.r = 0.8f;
	for (int i = 0; i < 6; i++)
	{
		discs[i] = -1;
	}
}

Tower::~Tower()
{
}

void Tower::PrintTowerLocations()
{
	for (int i = 0; i <= topDiscIndex; i++)
	{
		printf(" %d ", discs[i]);
	}
}

bool Tower::PlaceDiscOnTower(int _discNumber)
{
	// If we are at index -1, there are no discs on this tower yet
	if (topDiscIndex == -1)
	{
		discs[0] = _discNumber;
		topDiscIndex = 0;
	}
	// If the top disc has a larger index then the disc the user wants
	// to place here, then they cannot as that would be a larger disc on top
	// of a smaller disc
	else if (discs[topDiscIndex] > _discNumber)
	{
		return false;
	}
	// If the top index is 5, this tower is full
	else if (topDiscIndex == 5)
	{
		return false;
	}
	// Otherwise, the disc can go here so increment top index tracker and place disc here 
	else
	{
		topDiscIndex++;
		discs[topDiscIndex] = _discNumber;
	}
	return true;
}

void Tower::ClearDiscs()
{
	while (topDiscIndex != -1)
	{
		RemoveTopDisc();
	}
}

void Tower::RemoveTopDisc()
{
	// Ensure the tower is not empty
	if (topDiscIndex != -1)
	{
		// Then set the top value to -1
		discs[topDiscIndex] = -1;
		// And decrement top index tracker
		topDiscIndex--;
	}
}

void Tower::FillDiscArrayToIndex(int _index)
{
	for (int i = 0; i < _index; i++)
	{
		discs[i] = i;
		topDiscIndex = i;
	}
}

void Tower::Draw()
{
	if (isVisible)
	{
		glPushMatrix();
			glTranslatef(xCentre, yCentre, 0);
			glColor3f(color.r, color.g, color.b);

			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(-10, 0, -4);
			glVertex3f(10, 0, -4);
			glVertex3f(10, 100, -4);
			glVertex3f(-10, 100, -4);
			glVertex3f(-10, 0, -4);
			glEnd();
		glPopMatrix();
	}
}

