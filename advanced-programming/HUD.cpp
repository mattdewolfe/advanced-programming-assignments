#include "HUD.h"

HUD::HUD(void)
{
	hudFont = (long)GLUT_BITMAP_8_BY_13;
}
// general hud draw call
void HUD::Draw(int _score, int _scans)
{
	glPushMatrix();
	WriteData(_score, _scans);
	glPopMatrix();
}

// routine to draw a bitmap character string
void HUD::WriteBitmapString(void *font, char *string)
{  
   char *c;
   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}
// routine to onvert floating point to char string
void HUD::FloatToString(char * destStr, int precision, int val) 
{
   sprintf(destStr,"%d",val);
   destStr[precision] = '\0';
}

// write standard text for ui elements
void HUD::WriteData(int _score, int _scans)
{
	glColor3f(1.0, 0, 0);
	glRasterPos3f(4.2f, 193.0f, -1.0f);
	WriteBitmapString((void*)hudFont, "REMAINING SCANS: ");
	FloatToString(hudStringBuffer, 8, _scans);
	WriteBitmapString((void*)hudFont, hudStringBuffer);
	
	glColor3f(1.0, 0, 0);
	glRasterPos3f(GLUT_SCREEN_WIDTH/8, 189.0f, -1.0f);
	WriteBitmapString((void*)hudFont, "SCORE: ");
	FloatToString(hudStringBuffer, 8, _score);
	WriteBitmapString((void*)hudFont, hudStringBuffer);

	if (isExtractMode == true)
	{
		glRasterPos3f(GLUT_SCREEN_WIDTH + 25 , 192.0f, -1.0f);
		WriteBitmapString((void*)hudFont, "EXTRACT MODE");
	}
	else
	{
		glRasterPos3f(GLUT_SCREEN_WIDTH + 27 , 192.0f, -1.0f);
		WriteBitmapString((void*)hudFont, "SCAN MODE");
	}
}

HUD::~HUD(void)
{
}
