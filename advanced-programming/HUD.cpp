#include "HUD.h"

HUD::HUD(void)
{
	hudFont = (long)GLUT_BITMAP_8_BY_13;
	lasersShot = LASERFRAMES;
}
// general hud draw call
void HUD::Draw()
{
	glPushMatrix();
	WriteData();
	DrawWeapons();
	glPopMatrix();
}
// draw weapons to hud, and lasers if need be
void HUD::DrawWeapons()
{
	glPushMatrix();
	glTranslatef(0, 0, -1.0f);
#pragma region CenterLeftLaser
		// center laser - left
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(-1.0, 100.0, 0);
			glVertex3f(4.0, 105.0, 0);
			glVertex3f(14.0, 105.0, 0);
			glVertex3f(9.0, 100.0, 0);
			glVertex3f(-1.0, 100.0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3f(14.0, 105.0, 0);
			glVertex3f(14.0, 103.0, 0);
			glVertex3f(11.0, 98.0, 0);
			glVertex3f(11.0, 93.0, 0);
			glVertex3f(14.0, 88.0, 0);
			glVertex3f(14.0, 85.0, 0);
			glVertex3f(9.0, 90.0, 0);
			glVertex3f(9.0, 100.0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3f(9.0, 90.0, 0);
			glVertex3f(-1.0, 90.0, 0);
			glVertex3f(4.0, 85.0, 0);
			glVertex3f(14.0, 85.0, 0);
		glEnd();
		glColor3f(0.3, 0.3, 0.8);
		glBegin(GL_LINES);
			glVertex3f(0.0, 95.0, 0);
			glVertex3f(4.0, 98.0, 0);
			glVertex3f(4.0, 98.0, 0);
			glVertex3f(5.0, 96.0, 0);
			glVertex3f(5.0, 96.0, 0);
			glVertex3f(1.0, 93.0, 0);
			glVertex3f(5.0, 96.0, 0);
			glVertex3f(5.0, 94.0, 0);
			glVertex3f(5.0, 94.0, 0);
			glVertex3f(2.0, 91.0, 0);
		glEnd();
#pragma endregion CenterLeftLaser
#pragma region LowerLeftLaser
		// lower laser - left
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(-1.0, 8.0, 0);
			glVertex3f(4.0, 13.0, 0);
			glVertex3f(14.0, 13.0, 0);
			glVertex3f(9.0, 8.0, 0);
			glVertex3f(-1.0, 8.0, 0);
		glEnd();
	glBegin(GL_LINE_STRIP);
			glVertex3f(14.0, 13.0, 0);
			glVertex3f(14.0, 11.0, 0);
			glVertex3f(11.0, 8.0, 0);
			glVertex3f(11.0, 3.0, 0);
			glVertex3f(14.0, -2.0, 0);
			glVertex3f(14.0, -5.0, 0);
			glVertex3f(9.0, 0.0, 0);
			glVertex3f(9.0, 8.0, 0);
		glEnd();
		glColor3f(0.3, 0.3, 0.8);
		glBegin(GL_LINES);
			glVertex3f(0.0, 3.0, 0);
			glVertex3f(4.0, 6.0, 0);
			glVertex3f(4.0, 6.0, 0);
			glVertex3f(5.0, 4.0, 0);
			glVertex3f(5.0, 4.0, 0);
			glVertex3f(1.0, 1.0, 0);
			glVertex3f(5.0, 4.0, 0);
			glVertex3f(5.0, 2.0, 0);
			glVertex3f(5.0, 2.0, 0);
			glVertex3f(2.0, -1.0, 0);
		glEnd();
#pragma endregion LowerLeftLaser
	glTranslatef(29, 0, 0);
#pragma region Nose
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
		glVertex3f(85.0f, 0.0f, 0.0f);
		glVertex3f(95.0f, 10.0f, 0.0f);
		glVertex3f(95.0f, 10.0f, 0.0f);
		glVertex3f(105.0f, 10.0f, 0.0f);
		glVertex3f(105.0f, 10.0f, 0.0f);
		glVertex3f(115.0f, 0.0f, 0.0f);
		glVertex3f(95.0f, 10.0f, 0.0f);
		glVertex3f(92.0f, 0.0f, 0.0f);
		glVertex3f(105.0f, 10.0f, 0.0f);
		glVertex3f(108.0f, 0.0f, 0.0f);
	glColor3f(0.0, 0.0, 0.8);
		glVertex3f(95.0f, 5.0f, 0.0f);
		glVertex3f(105.0f, 5.0f, 0.0f);
		glVertex3f(95.0f, 5.0f, 0.0f);
		glVertex3f(92.0f, 0.0f, 0.0f);
		glVertex3f(105.0f, 5.0f, 0.0f);
		glVertex3f(108.0f, 0.0f, 0.0f);
	glEnd();
#pragma endregion Nose
#pragma region Shields	
	// shields visual
		glColor3f(0.0, 0.8, 0.0);
		glBegin(GL_LINE_STRIP);
		glVertex3f(80, 185, 0);
		glVertex3f(100, 195, 0);
		glVertex3f(120, 185, 0);
		glVertex3f(120, 195, 0);
		glVertex3f(80, 195, 0);
		glVertex3f(80, 185, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(87, 189, 0);
		glVertex3f(87, 195, 0);
		glVertex3f(95, 193, 0);
		glVertex3f(95, 195, 0);
		glVertex3f(106, 193, 0);
		glVertex3f(106, 195, 0);
		glVertex3f(114, 189, 0);
		glVertex3f(114, 195, 0);
		glEnd();
#pragma endregion Shields
	glTranslatef(37, 0, 0);
#pragma region CenterRightLaser
		// center laser - left
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(201.0, 100.0, 0);
			glVertex3f(196.0, 105.0, 0);
			glVertex3f(186.0, 105.0, 0);
			glVertex3f(191.0, 100.0, 0);
			glVertex3f(201.0, 100.0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3f(186.0, 105.0, 0);
			glVertex3f(186.0, 103.0, 0);
			glVertex3f(189.0, 98.0, 0);
			glVertex3f(189.0, 93.0, 0);
			glVertex3f(186.0, 88.0, 0);
			glVertex3f(186.0, 85.0, 0);
			glVertex3f(191.0, 90.0, 0);
			glVertex3f(191.0, 100.0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3f(191.0, 90.0, 0);
			glVertex3f(201.0, 90.0, 0);
			glVertex3f(196.0, 85.0, 0);
			glVertex3f(186.0, 85.0, 0);
		glEnd();
		glColor3f(0.3, 0.3, 0.8);
		glBegin(GL_LINES);
			glVertex3f(200.0, 95.0, 0);
			glVertex3f(196.0, 98.0, 0);
			glVertex3f(196.0, 98.0, 0);
			glVertex3f(195.0, 96.0, 0);
			glVertex3f(195.0, 96.0, 0);
			glVertex3f(199.0, 93.0, 0);
			glVertex3f(195.0, 96.0, 0);
			glVertex3f(195.0, 94.0, 0);
			glVertex3f(195.0, 94.0, 0);
			glVertex3f(198.0, 91.0, 0);
		glEnd();
#pragma endregion CenterRightLaser
#pragma region LowerRightLaser
		// center laser - left
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(201.0, 8.0, 0);
			glVertex3f(196.0, 13.0, 0);
			glVertex3f(186.0, 13.0, 0);
			glVertex3f(191.0, 8.0, 0);
			glVertex3f(201.0, 8.0, 0);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3f(186.0, 13.0, 0);
			glVertex3f(186.0, 11.0, 0);
			glVertex3f(189.0, 6.0, 0);
			glVertex3f(189.0, 1.0, 0);
			glVertex3f(186.0, -4.0, 0);
			glVertex3f(186.0, -7.0, 0);
			glVertex3f(191.0, -2.0, 0);
			glVertex3f(191.0, 8.0, 0);
		glEnd();
		glColor3f(0.3, 0.3, 0.8);
		glBegin(GL_LINES);
			glVertex3f(200.0, 3.0, 0);
			glVertex3f(196.0, 6.0, 0);
			glVertex3f(196.0, 6.0, 0);
			glVertex3f(195.0, 4.0, 0);
			glVertex3f(195.0, 4.0, 0);
			glVertex3f(199.0, 1.0, 0);
			glVertex3f(195.0, 4.0, 0);
			glVertex3f(195.0, 2.0, 0);
			glVertex3f(195.0, 2.0, 0);
			glVertex3f(198.0, -1.0, 0);
		glEnd();
#pragma endregion LowerRightLaser
	glPopMatrix();
#pragma region LaserVisuals
	if (lasersShot < LASERFRAMES)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				glColor3f(0.3, 0.3, 1.0);
				glLineWidth(2.0);
				glBegin(GL_LINES);
					glVertex3f(10 + (i * 245), 98 - (j * 90), -1);
					glVertex3f(laserX, laserY, -1);
				glEnd();
				glLineWidth(1.0);
			}
		}
		lasersShot++;
	}
#pragma endregion LaserVisuals
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
void HUD::WriteData()
{
	glColor3f(1.0, 0, 0);
	glRasterPos3f(GLUT_SCREEN_WIDTH/8 - 5, 192.0f, -1.0f);
	WriteBitmapString((void*)hudFont, "SCORE ");
	/*	floatToString(hudStringBuffer, 8, playerScore->GetScore());
		glRasterPos3f(screenWidth - 50, screenHeight - 6, 0);
		writeBitmapString((void*)font, "SCORE: ");
		writeBitmapString((void*)font, theStringBuffer);*/
	glRasterPos3f(123.0f , 185.0f, -1.0f);
	WriteBitmapString((void*)hudFont, "SHIELD ");
	glRasterPos3f(GLUT_SCREEN_WIDTH + 25 , 192.0f, -1.0f);
	WriteBitmapString((void*)hudFont, "WAVE ");
}
// change values for laser firing
void HUD::FireLaser(float x, float y)
{
	laserX = x;
	laserY = y;
	lasersShot = 0;
}
HUD::~HUD(void)
{
}
