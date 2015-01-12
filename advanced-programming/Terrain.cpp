#include "Terrain.h"

Terrain::Terrain(void) : VisualObject(0, 0, 1)
{
}

void Terrain::DrawSurface()
{
	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glTranslatef(0, -scaleZ, 0);
	glBegin(GL_LINES);
		glVertex3f(-400, 2, -1);
		glVertex3f(500, 2, -1);
	glEnd();
	if (scaleZ > 12)
	{
		glBegin(GL_LINES);
			glVertex3f(-200, scaleZ, -1);
			glVertex3f(500, scaleZ, -1);
		glEnd();
	}
	if (scaleZ > 29)
		scaleZ = 1;
	glPopMatrix();
}
void Terrain::DrawTrench()
{
	glPushMatrix();
	glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_LINES);
	// left trench wall
		glVertex3f(30, 0, -1);
		glVertex3f(130, 80, -1);
		glVertex3f(0, 80, -1);
		glVertex3f(130, 80, -1);
		
	// right trench wall
		glVertex3f(236, 0, -1);
		glVertex3f(130, 80, -1);
		glVertex3f(266, 80, -1);
		glVertex3f(130, 80, -1);
	glEnd();
}
Terrain::~Terrain(void)
{
}
