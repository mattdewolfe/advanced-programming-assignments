#ifndef HUD_H
#define HUD_H
#include <stdlib.h>
#include <iostream>
#include "openGL\glut.h"
#define LASERFRAMES 10
class HUD
{
public:
	HUD(void);
	// draw the hud based on the current stage
	void Draw();
	virtual ~HUD(void);
	// fire players weapon
	void FireLaser(float x, float y);
private:
	int lasersShot;
	float laserX, laserY;
	long hudFont; // font selection
	char hudStringBuffer[10]; // string buffer
	void DrawWeapons();
	void HUD::WriteBitmapString(void *font, char *string);
	void HUD::FloatToString(char * destStr, int precision, int val);
	void HUD::WriteData();

};

#endif

