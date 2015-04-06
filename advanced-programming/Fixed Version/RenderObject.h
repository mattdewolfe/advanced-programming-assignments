#pragma once
#include <iostream>
#include "openGL\glut.h"
class RenderObject
{
public:
	RenderObject(float _xCentre = 0, float _yCentre = 0);
	virtual ~RenderObject();

	void SetIsVisible(bool _isVis) { isVisible = _isVis; }
	bool IsVisibile() { return isVisible; }

	virtual void Draw() {}

	struct Color
	{
		Color(float _r = 0, float _g = 0, float _b = 0)
		{
			r = _r;
			g = _g;
			b = _b;
		}
		float r;
		float g;
		float b;
	};

protected:
	bool isVisible;
	float xCentre, yCentre;
	Color color;
	
};

