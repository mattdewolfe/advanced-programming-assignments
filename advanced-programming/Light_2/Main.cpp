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

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

// Total verts for
// (6 faces)(2 triangles/face)(3 vertices/triangle) = 36 verts per quad
// 3 towers, 6 discs = 9*36 = 324
// 
const int NumVertices = 324; 

point4 points[NumVertices];
vec3  normals[NumVertices];

// Vertices of the towers
point4 towerVertices[8] = {
	point4(-0.05, -0.4, 0.05, 1.0),
	point4(-0.05, 0.4, 0.05, 1.0),
	point4(0.05, 0.4, 0.05, 1.0),
	point4(0.05, -0.4, 0.05, 1.0),
	point4(-0.05, -0.4, -0.05, 1.0),
	point4(-0.05, 0.4, -0.05, 1.0),
	point4(0.05, 0.4, -0.05, 1.0),
	point4(0.05, -0.4, -0.05, 1.0)
};

// Vertices of the discs
point4 discVertices[8] = {
	point4(-0.2, -0.05, 0.2, 1.0),
	point4(-0.2, 0.05, 0.2, 1.0),
	point4(0.2, 0.05, 0.2, 1.0),
	point4(0.2, -0.05, 0.2, 1.0),
	point4(-0.2, -0.05, -0.2, 1.0),
	point4(-0.2, 0.05, -0.2, 1.0),
	point4(0.2, 0.05, -0.2, 1.0),
	point4(0.2, -0.05, -0.2, 1.0)
};

// Current disk the player is moving
int selectedDisc;
// Store current level player is on
int currentLevel;

// Enum of gameplay states
enum PlayState 
{
	Begin = 0, 
	SelectDisc, 
	SelectTower, 
	DiscMoving
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

//----------------------------------------------------------------------------
// quad generates two triangles for each face and assigns colors to the vertices
int Index = 0;

void towerQuad(int a, int b, int c, int d, float offset)
{
	// Initialize temporary vectors along the quad's edge to
	// compute its face normal 
	point4 tempPoints[4];
	tempPoints[0] = towerVertices[a];
	tempPoints[0].x += offset;

	tempPoints[1] = towerVertices[b];
	tempPoints[1].x += offset;

	tempPoints[2] = towerVertices[c];
	tempPoints[2].x += offset;

	tempPoints[3] = towerVertices[d];
	tempPoints[3].x += offset;
	/*
	vec4 u = towerVertices[b] - towerVertices[a];
	vec4 v = towerVertices[c] - towerVertices[b];
	*/
	vec4 u = towerVertices[1] - towerVertices[0];
	vec4 v = towerVertices[2] - towerVertices[1];

	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal; points[Index] = tempPoints[0]; Index++;
	normals[Index] = normal; points[Index] = tempPoints[1]; Index++;
	normals[Index] = normal; points[Index] = tempPoints[2]; Index++;
	normals[Index] = normal; points[Index] = tempPoints[0]; Index++;
	normals[Index] = normal; points[Index] = tempPoints[2]; Index++;
	normals[Index] = normal; points[Index] = tempPoints[3]; Index++;
}

// generate 12 triangles: 36 vertices and 36 colors
void tower(float offset = 0.0f)
{
	towerQuad(1, 0, 3, 2, offset);
	towerQuad(2, 3, 7, 6, offset);
	towerQuad(3, 0, 4, 7, offset);
	towerQuad(6, 5, 1, 2, offset);
	towerQuad(4, 5, 6, 7, offset);
	towerQuad(5, 4, 0, 1, offset);
}

// Draw calls for the discs
void discQuad(int a, int b, int c, int d, float offset)
{
	// Initialize temporary vectors along the quad's edge to
	// compute its face normal 
	point4 tempPoints[4];

	tempPoints[0] = discVertices[a];
	tempPoints[0].x -= 0.5f;
	tempPoints[0].y += offset;

	tempPoints[1] = discVertices[b];
	tempPoints[1].x -= 0.5f;
	tempPoints[1].y += offset;

	tempPoints[2] = discVertices[c];
	tempPoints[2].x -= 0.5f;
	tempPoints[2].y += offset;

	tempPoints[3] = discVertices[d];
	tempPoints[3].x -= 0.5f;
	tempPoints[3].y += offset;

	vec4 u = tempPoints[1] - tempPoints[0];
	vec4 v = tempPoints[2] - tempPoints[1];

	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal; points[Index] = tempPoints[0]; Index++;
	normals[Index] = normal; points[Index] = tempPoints[1]; Index++;
	normals[Index] = normal; points[Index] = tempPoints[2]; Index++;
	normals[Index] = normal; points[Index] = tempPoints[0]; Index++;
	normals[Index] = normal; points[Index] = tempPoints[2]; Index++;
	normals[Index] = normal; points[Index] = tempPoints[3]; Index++;
}

// generate 12 triangles: 36 vertices and 36 colors
void disc(float offset = 0.0f)
{
	discQuad(1, 0, 3, 2, offset);
	discQuad(2, 3, 7, 6, offset);
	discQuad(3, 0, 4, 7, offset);
	discQuad(6, 5, 1, 2, offset);
	discQuad(4, 5, 6, 7, offset);
	discQuad(5, 4, 0, 1, offset);
}

// OpenGL initialization
void init()
{
	float tempOffset = -0.5f;
	for (int i = 0; i < 3; i++)
	{
		tower(tempOffset);
		tempOffset += 0.5f;
	}
	tempOffset = -0.4f;
	for (int i = 0; i < 6; i++)
	{
		disc(tempOffset);
		tempOffset += 0.1;
	}
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),
		sizeof(normals), normals);

	// Load shaders and use the resulting shader program
	// GLuint program = InitShader("vshader53.glsl", "fshader53.glsl");

	ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "vshader53.glsl" },
			{ GL_FRAGMENT_SHADER, "fshader53.glsl" },
			{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);

	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	// Initialize shader lighting parameters
	point4 light_position(0.0, 0.0, 0.0, 0.0);
	color4 light_ambient(1.0, 0.0, 0.0, 1.0);
	color4 other_ambient_light(0.0, 1.0, 0.0, 1.0);
	color4 light_diffuse(0.0, 1.0, 0.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(1.0, 0.0, 0.0, 1.0);
	color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
	color4 material_specular(1.0, 0.8, 0.0, 1.0);
	
	float  material_shininess = 100.0;
		
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

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

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
	Theta[Axis] += 0.0;

	if (Theta[Axis] > 360.0) 
	{
		Theta[Axis] -= 360.0;
	}

	glutPostRedisplay();
}

// Function for handling key presses that manipulate towers
void handleKeyPress(unsigned char _key)
{
	// Case for selecing a disc to move
	if (state == SelectDisc)
	{
		switch (_key)
		{
		case '1':
			// selectedDisc = tower[0].TopDisc();
			break;

		case '2':
			// selectedDisc = tower[1].TopDisc();
			break;

		case '3':
			// selectedDisc = tower[2].TopDisc();
			break;

		case '4':
			if (currentLevel > 1)
			{
				// selectedDisc = tower[3].TopDisc();
			}
			break;
		}
	}
	// Case for selecing a tower to place the disc on
	else if (state == SelectTower)
	{
		switch (_key)
		{
		case '1':
			// selectedDisc = tower[0].TopDisc();
			break;

		case '2':
			// selectedDisc = tower[1].TopDisc();
			break;

		case '3':
			// selectedDisc = tower[2].TopDisc();
			break;

		case '4':
			if (currentLevel > 1)
			{
				// selectedDisc = tower[3].TopDisc();
			}
			break;
		}
	}
	else if (state == DiscMoving)
	{}

}

// Callback function for glut keyboard
void keyboard(unsigned char key, int x, int y)
{
	switch (key) 
	{
	case '1':
	case '2':
	case '3':
	case '4':
		handleKeyPress(key);
		break;
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
	}
}

//----------------------------------------------------------------------------

void
reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	GLfloat aspect = GLfloat(width) / height;
	mat4 projection = Perspective(45.0, aspect, 0.5, 3.0);

	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	//glutInitContextVersion(3, 2);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Color Cube");

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
