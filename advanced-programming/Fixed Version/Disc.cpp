#include "Disc.h"


Disc::Disc(float _xPos, float _yPos) : RenderObject(_xPos, _yPos),
targetX(_xPos),
targetY(_yPos)
{
}

void Disc::SetSize(int _size)
{
	color.r = 0.2;
	switch (_size)
	{
	case 0:
		color.g = 1.0f;
		color.b = 0.1f;
		break;

	case 1:
		color.g = 0.8f;
		color.b = 0.25f;
		break;

	case 2:
		color.g = 0.6f;
		color.b = 0.4f;
		break;

	case 3:
		color.g = 0.4f;
		color.b = 0.6f;
		break;

	case 4:
		color.g = 0.2f;
		color.b = 0.8f;
		break;

	case 5:
		color.g = 0;
		color.b = 1.0f;
		break;

	}

	width = 22 - (_size);
}

Disc::~Disc()
{
}

void Disc::SetTargetLocation(float _x, float _y)
{
	targetX = _x;
	targetY = _y;
	isMoving = true;
}

void Disc::Update()
{
	if (isMoving == true)
	{
		if (targetX != xCentre)
		{
			if (yCentre < 155)
			{
				yCentre += 1.0f;
			}
			else if (targetX > xCentre)
			{ 
				xCentre += 1.0f;
			}
			else
			{
				xCentre -= 1.0f;
			}
		}
		else if (targetY < yCentre)
		{
			yCentre -= 1.0f;
		}
		else
		{
			isMoving = false;
		}
	}
}

void Disc::Draw()
{
	if (isVisible)
	{
		glPushMatrix();
			glTranslatef(xCentre, yCentre, 0);
			glColor3f(color.r, color.g, color.b);

			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(-width, 0, -3);
			glVertex3f(width, 0, -3);
			glVertex3f(width, 12, -3);
			glVertex3f(-width, 12, -3);
			glVertex3f(-width, 0, -3);
			glEnd();
		glPopMatrix();
	}
}
