#include "HUD.h"

HUD::HUD(void)
{
	hudFont = (long)GLUT_BITMAP_8_BY_13;
}
// general hud draw call
void HUD::Draw(int _moves, int _towerCount, int _selectedDisc)
{
	glPushMatrix();
	WriteData(_moves, _towerCount, _selectedDisc);
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
void HUD::WriteData(int _moves, int _towerCount, int _selectedDisc)
{
	glColor3f(1.0, 0, 0);
	glRasterPos3f(GLUT_SCREEN_WIDTH/10, 189.0f, -1.0f);
	WriteBitmapString((void*)hudFont, "MOVE COUNT: ");
	FloatToString(hudStringBuffer, 8, _moves);
	WriteBitmapString((void*)hudFont, hudStringBuffer);

	for (int i = 0; i < _towerCount; i++)
	{
		glRasterPos3f(30 +(i*60), 170.0f, -1.0f);
		WriteBitmapString((void*)hudFont, "#");
		FloatToString(hudStringBuffer, 8, i+1);
		WriteBitmapString((void*)hudFont, hudStringBuffer);
	}

	if (_selectedDisc != -1)
	{
		glRasterPos3f(GLUT_SCREEN_WIDTH - (GLUT_SCREEN_WIDTH / 9), 189.0f, -1.0f);
		WriteBitmapString((void*)hudFont, "SELECTED DISC: ");
		FloatToString(hudStringBuffer, 8, _selectedDisc);
		WriteBitmapString((void*)hudFont, hudStringBuffer);
	}
}

HUD::~HUD(void)
{
}
