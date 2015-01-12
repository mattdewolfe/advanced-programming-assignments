#ifndef TERRAIN_H
#define TERRAIN_H

#include "VisualObject.h"
#include "openGL\glut.h"

class Terrain : public VisualObject
{
public:
	Terrain(void);
	virtual ~Terrain(void);
	void DrawSurface();
	void DrawTrench();
};
#endif

