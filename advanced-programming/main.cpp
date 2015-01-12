#include <cstdlib>
#include <cmath>
#include <iostream>

#include "GameLogic.h"
#include "VisualObject.h"
#include "openGL/glut.h"

#define PI 3.14159265
							 
// global variables
static int animationPeriod = 33; // Time between draw calls
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection
static int width, height; // Size of the OpenGL window
static float mouseX, mouseY;

static GameManager gameManager;

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
}

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;
   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
} 

// Initialization routine
void setup(void) 
{
   glEnable(GL_DEPTH_TEST);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glClearColor (0.0, 0.0, 0.0, 0.0);
}

void DrawCrossHair()
{
	glPushMatrix();
		glColor3f(0.8, 0.8, 0.0);
		glTranslatef(mouseX, mouseY, -1.0f);
		glBegin(GL_TRIANGLES);
		// upper left crosshair arrow
		glVertex3f(-3, 4, 0);
		glVertex3f(-0.5, 0.5, 0);
		glVertex3f(-4, 3, 0);
		// bottom left crosshair arrow
		glVertex3f(-3, -4, 0);
		glVertex3f(-0.5, -0.5, 0);
		glVertex3f(-4, -3, 0);
		// upper right crosshair arrow
		glVertex3f(3, 4, 0);
		glVertex3f(0.5, 0.5, 0);
		glVertex3f(4, 3, 0);
		// bottom right crosshair arrow
		glVertex3f(3, -4, 0);
		glVertex3f(0.5, -0.5, 0);
		glVertex3f(4, -3, 0);
	/*	glVertex3f(-2, -2, 0);
		glVertex3f(-2, 2, 0);
		glVertex3f(2, 2, 0);*/
		glEnd();
	glPopMatrix();
}

// Drawing routine
void drawScene(void)
{  
	int i, j;
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	DrawCrossHair();
	gameManager.DrawVisuals();
	gameManager.Update();
	glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport (0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 266.0, 0.0, 200.0, 1.0, 300.0);
   glMatrixMode(GL_MODELVIEW);
   // Pass the size of the OpenGL window
   width = w;
   height = h;
}
// Mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
	// if left button is released, pass values to gameManager to make use of
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		gameManager.MousePress(x/3.84, 200 - y/3.84);
}
// Mouse motion callback routine (for click and hold functionality)
void mouseMotion(int x, int y)
{
}
// Update mouse position regardless of button presses
void passiveMotionFunc(int x, int y)
{
	// convert mouse cursor position into screen coordinates
	mouseX = x/3.84;
	mouseY = 200 - y/3.84;
}
// Keyboard input processing routine
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}
// Callback routine for non-ASCII key entry
void specialKeyInput(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT) 
	{ 
		if (gameManager.moveLeft == false)
			gameManager.moveLeft = true;
	}
	if (key == GLUT_KEY_RIGHT) 
	{ 
		if (gameManager.moveRight == false)
			gameManager.moveRight = true;
	}
		if (key == GLUT_KEY_DOWN) 
	{ 
		if (gameManager.moveDown == false)
			gameManager.moveDown = true;
	}
	if (key == GLUT_KEY_UP) 
	{ 
		if (gameManager.moveUp == false)
			gameManager.moveUp = true;
	}
}
// Callback routine for non-ASCII key release
void specialKeyUp(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT) 
	{ 
		if (gameManager.moveLeft == true)
			gameManager.moveLeft = false;
	}
	if (key == GLUT_KEY_RIGHT) 
	{ 
		if (gameManager.moveRight == true)
			gameManager.moveRight = false;
	}
		if (key == GLUT_KEY_DOWN) 
	{ 
		if (gameManager.moveDown == true)
			gameManager.moveDown = false;
	}
	if (key == GLUT_KEY_UP) 
	{ 
		if (gameManager.moveUp == true)
			gameManager.moveUp = false;
	}
}
// animation timer
void animate(int value)
{
 	glutPostRedisplay();
	glutTimerFunc(animationPeriod, animate, 1);
}
// Main routine
int main(int argc, char **argv) 
{
	printInteraction();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(250, 150); 
	glutCreateWindow("War of Stars");
	setup(); 
	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize);  
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glutSpecialUpFunc(specialKeyUp);
	// Mouse button push callback
	glutMouseFunc(mouseControl); 
	// Position of mouse cursor as dragged
	glutMotionFunc(mouseMotion);
	// Position of mouse cursor without clicks
	glutPassiveMotionFunc(passiveMotionFunc);
	glutTimerFunc(animationPeriod, animate, 1);
	glutMainLoop(); 
	return 0;  
}

/*
// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1, 
						     float x2, float y2, float z2, float r2)
{
   return ( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) <= (r1+r2)*(r1+r2) );
}

// Function to check if the spacecraft collides with an asteroid when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
int asteroidCraftCollision(float x, float z, float a)
{
   int i,j;

   // Check for collision with each asteroid.
   for (j=0; j<COLUMNS; j++)
      for (i=0; i<ROWS; i++)
		 if (arrayAsteroids[i][j].getRadius() > 0 ) // If asteroid exists.
            if ( checkSpheresIntersection( x - 5 * sin( (PI/180.0) * a), 0.0, 
		         z - 5 * cos( (PI/180.0) * a), 7.072, 
		         arrayAsteroids[i][j].getCenterX(), arrayAsteroids[i][j].getCenterY(), 
		         arrayAsteroids[i][j].getCenterZ(), arrayAsteroids[i][j].getRadius() ) )
		       return 1;
   return 0;
}*/