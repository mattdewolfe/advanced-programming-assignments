#include <cstdlib>
#include <cmath>
#include <iostream>

#include "GameManager.h"
#include "openGL/glut.h"

#define PI 3.14159265
							 
// global variables
static int animationPeriod = 33; // Time between draw calls
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection
static int width, height; // Size of the OpenGL window
static float mouseX, mouseY;
static float ratioX, ratioY;

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
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glClearColor (0.0, 0.0, 0.0, 0.0);
}

// Drawing routine
void drawScene(void)
{  
	int i, j;
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
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
   std::cout << h;
	
}
// Mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
	// if left button is released, pass values to gameManager to make use of
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		gameManager.MousePress(x/ratioX, 200 - y/ratioY);
}
// Mouse motion callback routine (for click and hold functionality)
void mouseMotion(int x, int y)
{

}
// Update mouse position regardless of button presses
void passiveMotionFunc(int x, int y)
{
	// convert mouse cursor position into screen coordinates
	mouseX = x/ratioX;
	mouseY = 200 - y/ratioY;
}
// Keyboard input processing routine
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:
         exit(0);
         break;
	  case 32:
		  gameManager.ToggleMode();
      default:
         break;
   }
}
// Callback routine for non-ASCII key entry
void specialKeyInput(int key, int x, int y)
{}
// Callback routine for non-ASCII key release
void specialKeyUp(int key, int x, int y)
{}
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
	ratioX = 3.84;
	ratioY = 3.84;
	glutCreateWindow("Harvester");
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