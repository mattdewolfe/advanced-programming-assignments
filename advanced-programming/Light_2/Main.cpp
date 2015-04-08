//
//  Display a rotating cube with lighting
//
//  Light and material properties are sent to the shader as uniform
//    variables.  Vertex positions and normals are sent after each
//    rotation.

#include "Angel.h"
#include "GL/glut.h"
#include "LoadShaders.h"
#include "vgl.h"
#include "Tower.h"
#include "Disc.h"

// Current disc the player is moving
int selectedDisc;
// The tower that the selected disc belongs to
int selectedDiscOwningTower;
// Current tower selected to move a disc off of
int selectedTower;
// Store current level player is on
int currentLevel;
// Number of discs in this level
int numberOfDiscs;
// Number of towers in this level
int numberOfTowers;
// Number of disc moves this level
int moveCount;
// Target tower of the disc that is moving
int targetTower;
// Screen space value between towers
float towerGap = 0.4f;
float towerStartPosition = -0.55f;

Tower towers[4];
Disc* discs[6];

void nextLevel();
void checkVictory();
void printDiscLocations();
void moveDisc();

// Enum of gameplay states
enum PlayState 
{
	Begin = 0, 
	SelectDisc, 
	SelectTower, 
	DiscMoving,
	LevelComplete
};
PlayState state;

// Array of rotation angles (in degrees) for each coordinate axis
enum 
{ 
	Xaxis = 0,
	Yaxis = 1, 
	Zaxis = 2, 
	NumAxes = 3 
};
int Axis = Yaxis;

GLfloat Theta[NumAxes] = { 0.0, 0.0, 0.0 };

// Model-view and projection matrices uniform location
GLuint ModelView, Projection;

// OpenGL initialization
void init()
{
	// alternate method
	// GLuint program = InitShader("vshader53.glsl", "fshader53.glsl");

	// Load shaders and program to use with towers
	ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "vshader53.glsl" },
			{ GL_FRAGMENT_SHADER, "fshader53.glsl" },
			{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);
	
	// Create towers
	float tempOffsetX = towerStartPosition;
	for (int i = 0; i < 4; i++)
	{
		towers[i].SetProgram(program);
		towers[i].Create(tempOffsetX);
		tempOffsetX += towerGap;
	}

	// Create discs
	tempOffsetX = towerStartPosition;
	float tempOffsetY = -0.4f;
	vec4 discColor = vec4(0.8f, 0.1f, 0.2f, 1.0f);
	for (int i = 0; i < 6; i++)
	{
		discs[i] = new Disc(i);
		discs[i]->InitVertArray(i);
		discs[i]->SetProgram(program);
		discs[i]->Create(tempOffsetX, tempOffsetY, discColor);
		discColor.y += 0.15f;
		discColor.x -= 0.1f;
		tempOffsetY += 0.08f;
	}

	point4 light_position(0.0, 0.0, 0.0, 0.0);
	color4 light_ambient(1.0, 0.0, 0.0, 1.0);
	color4 other_ambient_light(1.0, 1.0, 1.0, 1.0);
	color4 light_diffuse(0.0, 1.0, 0.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(1.0, 1.0, 0.0, 1.0);
	color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
	color4 material_specular(1.0, 0.8, 0.0, 1.0);
	
	float material_shininess = 100.0;
		
	color4 ambient_product = light_ambient * material_ambient;
	color4 other_ambient_light_product = other_ambient_light * material_ambient;

	color4 ambient_result = ambient_product + other_ambient_light_product;

	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),
		1, ambient_result);
	
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
		1, diffuse_product);
	
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
		1, specular_product);

	glUniform4fv(glGetUniformLocation(program, "LightPosition"),
		1, light_position);

	glUniform1f(glGetUniformLocation(program, "Shininess"),
		material_shininess);

	// Retrieve transformation uniform variable locations
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");

	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_FLAT);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	currentLevel = 0;
	nextLevel();
}

// Draw call
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Generate tha model-view matrix
	const vec3 viewer_pos(0.0, 0.0, 2.0);
	
	mat4 model_view = (Translate(-viewer_pos) *
		RotateX(Theta[Xaxis]) *
		RotateY(Theta[Yaxis]) *
		RotateZ(Theta[Zaxis]));

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
	
	// Iterate over all our objects and draw their verts
	for (int i = 0; i < numberOfTowers; i++)
	{
		towers[i].Update();
	}
	for (int i = 0; i < numberOfDiscs; i++)
	{
		discs[i]->Update();
	}
	glutSwapBuffers();
}

// Callback function for glut mouse listener
void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:    Theta[Axis] += 0.5f;  break;
		case GLUT_MIDDLE_BUTTON:  break;
		case GLUT_RIGHT_BUTTON:   Theta[Axis] -= 0.5f;  break;
		}
	}
}

// Loop function for glut
void idle(void)
{
	if (Theta[Axis] > 360.0) 
	{
		Theta[Axis] -= 360.0;
	}
	if (state == DiscMoving)
	{
		moveDisc();
	}
	else if (state == LevelComplete)
	{
		nextLevel();
	}
	else
	{
		checkVictory();
	}
	glutPostRedisplay();
}

// Function for handling key presses that manipulate towers
void handleKeyPress(unsigned char _key)
{
	int keyVal = -1;
	// Case for selecing a disc to move
	if (state == SelectDisc)
	{
		switch (_key)
		{
		case '1':
			keyVal = 0;
			break;

		case '2':
			keyVal = 1;
			break;

		case '3':
			keyVal = 2;
			break;

		case '4':
			if (currentLevel == 2)
			{
				keyVal = 3;
			}
			break;
		}
		if (keyVal != -1)
		{
			if (towers[keyVal].GetTopDisc() > -1)
			{
				selectedDisc = towers[keyVal].GetTopDisc();
				targetTower = -1;
				selectedDiscOwningTower = keyVal;
				state = SelectTower;
			}
		}
	}
	// Case for selecing a tower to place the disc on
	else if (state == SelectTower)
	{
		switch (_key)
		{
		case '1':
			keyVal = 0;
			break;

		case '2':
			keyVal = 1;
			break;

		case '3':
			keyVal = 2;
			break;

		case '4':
			if (currentLevel == 2)
			{
				keyVal = 3;
			}
			break;
		}
		// If the currently selected disc can go on this tower
		if (keyVal != -1 && towers[keyVal].PlaceDiscOnTower(selectedDisc) == true)
		{
			// Calc target X position
			float targetX = towerStartPosition + (towerGap*keyVal);
			// Calc target Y position
			float targetY = -0.4f;
			if (towers[keyVal].GetDiscCount() > 0)
			{
				targetY += (towers[keyVal].GetDiscCount()-1)*0.08;
			}
			// Pass new target positions to target disc
			discs[selectedDisc]->SetTargetOffsets(targetX, targetY);
			// Remove this disc from its old tower
			towers[selectedDiscOwningTower].RemoveTopDisc();
			// Update target tower to the specified tower
			targetTower = keyVal;
			// Increment move count for tracking
			moveCount++;
			// Switch to disc movement state
			state = DiscMoving;
		}
	}
	else if (state == DiscMoving)
	{}
	
	printDiscLocations();
}

// Moves the selected disc towards its new tower
void moveDisc()
{
	// Check to see if the current disc needs to continue moving
	if (discs[selectedDisc]->UpdatePositions() == false)
	{ 
		// If not change selection back to useless value
		selectedDisc = -1;
		// And switch back to selection state
		state = SelectDisc;
	}
}

// Check disc locations to see if player has won
void checkVictory()
{
	// Check if a given tower has every disc on it
	for (int i = 1; i < numberOfTowers; i++)
	{ 
		if (towers[i].GetDiscCount() >= numberOfDiscs)
		{
			state = LevelComplete;
		}
	}
}

// Callback function for glut keyboard
void keyboard(unsigned char key, int x, int y)
{
	switch (key) 
	{
	case 'p':
	case 'P':
		Theta[Axis] += 1.0f;
		break;
	case 'o':
	case 'O':
		Theta[Axis] -= 1.0f;
		break;
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	default:
		handleKeyPress(key);
		break;
	}
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	GLfloat aspect = GLfloat(width) / height;
	mat4 projection = Perspective(45.0, aspect, 0.5, 3.0);

	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

void printDiscLocations()
{
	system("cls");
	// If the level is not complete, print disc location info
	if (state != LevelComplete)
	{
		printf(" -- Current Disc Locations --\n");
		for (int i = 0; i < numberOfTowers; i++)
		{
			printf("Tower #%d: ", i + 1);
			towers[i].PrintTowerLocations();
			printf("\n");
		}
		printf(" -- Selected Disc: %d", selectedDisc);
		switch (state)
		{
		case SelectDisc:
			printf("\n\n Select a tower to take the top disc from...\n");
			break;
		case SelectTower:
			printf("\n\n Select a tower to place the selected disc onto...\n");
			break;
		default:
			break;
		}
	}
	// Otherwise print a level complete message
	else
	{
		printf(" -- Congratulations -- \n\n");
		printf(" -- You beat the level in %d moves!\n\n", moveCount);
		printf(" Press any key to continue.\n");
	}
}

void nextLevel()
{
	selectedDisc = 0;
	selectedDiscOwningTower = 0;
	selectedTower = 0;
	moveCount = 0;
	currentLevel += 1;
	
	switch (currentLevel)
	{
	case 1:
		numberOfDiscs = 4;
		numberOfTowers = 3;
		break;
	case 2:
		numberOfDiscs = 5;
		numberOfTowers = 4;
		break;
	case 3:
		numberOfDiscs = 6;
		numberOfTowers = 3;
		break;
	default:
		numberOfDiscs = 4;
		numberOfTowers = 3;
		break;
	}
	// Push discs onto leftmost tower
	towers[0].FillDiscArrayToIndex(numberOfDiscs);
	for (int i = 1; i < numberOfTowers; i++)
	{
		towers[i].ClearDiscs();
	}

	float tempOffsetY = -0.4f;
	for (int i = 0; i < numberOfDiscs; i++)
	{
		discs[i]->ResetPosition(towerStartPosition, tempOffsetY);
		tempOffsetY += 0.08f;
	}
	state = SelectDisc;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);

	glutCreateWindow("Towers of Hanoi");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}