#include "Tower.h"

Tower::Tower(float x, float y, float z) : VisualObject(x, y, z) 
{
	turretAlive = true;
}
void Tower::Draw()
{
	// store the interior values for connection lines between cubes
	float innerLarge, innerSmall;
	innerLarge = scaleZ/2;
	innerSmall = scaleZ/3;
	glColor3f(1.0, 0.1, 0.1);
	glPushMatrix();
	glTranslatef(posX + scaleZ, posY - scaleZ*2, 0);
	glBegin(GL_LINE_STRIP);
		glVertex3f(-innerLarge, 0, 0);
		glVertex3f(innerLarge, 0, 0);
		glVertex3f(innerLarge, scaleZ * 2, 0);
		glVertex3f(-innerLarge, scaleZ * 2, 0);
		glVertex3f(-innerLarge, 0, 0);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f(-innerSmall, innerSmall, 0);
		glVertex3f(innerSmall, innerSmall, 0);
		glVertex3f(innerSmall, scaleZ * 2 - innerSmall, 0);
		glVertex3f(-innerSmall, scaleZ * 2 - innerSmall, 0);
		glVertex3f(-innerSmall, innerSmall, 0);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(-innerLarge, 0, 0);
		glVertex3f(-innerSmall, innerSmall, 0);
		glVertex3f(innerLarge, 0, 0);
		glVertex3f(innerSmall, innerSmall, 0);
	glEnd();
	glTranslatef(0, scaleZ, 0);
	glBegin(GL_LINES);
		glVertex3f(-innerLarge, scaleZ , 0);
		glVertex3f(-innerSmall, scaleZ - innerSmall, 0);
		glVertex3f(innerLarge, scaleZ , 0);
		glVertex3f(innerSmall, scaleZ - innerSmall, 0);
	glEnd();
	// if the turret is alive, draw it
	if (turretAlive == true)
	{
		innerSmall = scaleZ/4;
		glColor3f(0.8, 0.8, 0);
		glTranslatef(0, scaleZ, 0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(-innerLarge, 0, 0);
			glVertex3f(innerLarge, 0, 0);
			glVertex3f(innerLarge, scaleZ, 0);
			glVertex3f(-innerLarge, scaleZ, 0);
			glVertex3f(-innerLarge, 0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3f(-innerSmall, innerSmall, 0);
			glVertex3f(innerSmall, innerSmall, 0);
			glVertex3f(innerSmall, scaleZ - innerSmall, 0);
			glVertex3f(-innerSmall, scaleZ - innerSmall, 0);
			glVertex3f(-innerSmall, innerSmall, 0);
		glEnd();
		glBegin(GL_LINES);
			glVertex3f(-innerLarge, 0, 0);
			glVertex3f(-innerSmall, innerSmall, 0);
			glVertex3f(innerLarge, 0, 0);
			glVertex3f(innerSmall, innerSmall, 0);
		glEnd();
		glBegin(GL_LINES);
			glVertex3f(-innerLarge, scaleZ , 0);
			glVertex3f(-innerSmall, scaleZ - innerSmall, 0);
			glVertex3f(innerLarge, scaleZ , 0);
			glVertex3f(innerSmall, scaleZ - innerSmall, 0);
		glEnd();
	}
	glPopMatrix();
}
Tower::~Tower(void)
{
}
