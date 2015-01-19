#ifndef HUD_H
#define HUD_H
#include <stdlib.h>
#include <iostream>
#include "openGL\glut.h"
class HUD
{
public:

	HUD(void);
	// draw the hud based on the current stage
	void Draw(int _score, int _scans);
	virtual ~HUD(void);
	// Switch between displaying extract and scan mode
	void ToggleMode() { isExtractMode = !isExtractMode; }

private:
	long hudFont; // font selection
	char hudStringBuffer[10]; // string buffer
	bool isExtractMode; // true when the player is in extract mode
	void HUD::WriteBitmapString(void *font, char *string);
	void HUD::FloatToString(char * destStr, int precision, int val);
	void HUD::WriteData(int _score, int _scans);

};

#endif

