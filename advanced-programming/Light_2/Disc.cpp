#include "Disc.h"

Disc::Disc(int _discSize) :
currentIndex(0),
bInitWasCalled(false),
size(_discSize),
isMoving(false)
{
	vPosition = 0;
	vNormal = 1;
	vColor = 2;
}

Disc::~Disc()
{}

void Disc::InitVertArray(int _discIndex)
{
	float discWidth = 0.18f;
	for (int i = 0; i < _discIndex; i++)
	{
		discWidth -= 0.02f;
	}
	discVertices[0] = point4(-discWidth, -0.035, discWidth, 1.0);
	discVertices[1] = point4(-discWidth, 0.035, discWidth, 1.0);
	discVertices[2] = point4(discWidth, 0.035, discWidth, 1.0);
	discVertices[3]	= point4(discWidth, -0.035, discWidth, 1.0);
	discVertices[4]	= point4(-discWidth, -0.035, -discWidth, 1.0);
	discVertices[5]	= point4(-discWidth, 0.035, -discWidth, 1.0);
	discVertices[6]	= point4(discWidth, 0.035, -discWidth, 1.0);
	discVertices[7]	= point4(discWidth, -0.035, -discWidth, 1.0);

	bInitWasCalled = true;
}

void Disc::DiscQuad(int _a, int _b, int _c, int _d)
{
	// Initialize temporary vectors along the quad's edge to
	// compute its face normal 
	point4 tempPoints[4];
	tempPoints[0] = discVertices[_a];

	tempPoints[1] = discVertices[_b];

	tempPoints[2] = discVertices[_c];

	tempPoints[3] = discVertices[_d];

	vec4 u = discVertices[1] - discVertices[0];
	vec4 v = discVertices[2] - discVertices[1];

	vec3 normal = normalize(cross(u, v));

	discNormals[currentIndex] = normal; discPoints[currentIndex] = tempPoints[0]; currentIndex++;
	discNormals[currentIndex] = normal; discPoints[currentIndex] = tempPoints[1]; currentIndex++;
	discNormals[currentIndex] = normal; discPoints[currentIndex] = tempPoints[2]; currentIndex++;
	discNormals[currentIndex] = normal; discPoints[currentIndex] = tempPoints[0]; currentIndex++;
	discNormals[currentIndex] = normal; discPoints[currentIndex] = tempPoints[2]; currentIndex++;
	discNormals[currentIndex] = normal; discPoints[currentIndex] = tempPoints[3]; currentIndex++;
}

void Disc::ResetPosition(float _xTarget, float _yTarget)
{
	currentLocation = vec4(_xTarget, _yTarget, 0, 0);
	targetLocation = currentLocation;
}

void Disc::SetTargetOffsets(float _xTarget, float _yTarget)
{
	targetLocation = vec4(_xTarget, _yTarget, 0, 0);
	
	isMoving = true;
}

bool Disc::UpdatePositions()
{
	if (isMoving == true)
	{
		// If X no longer matches
		float tempX = targetLocation.x - currentLocation.x;
		if (tempX < -0.002f || tempX > 0.002f)
		{
			// We first climb to a new Y value
			if (currentLocation.y < 0.5)
			{
				currentLocation.y += 0.0005f;
			}
			// From there we move across the screen to 
			else if (targetLocation.x > currentLocation.x)
			{ 
				currentLocation.x += 0.0005f;
			}
			else
			{
				currentLocation.x -= 0.0005f;
			}
		}
		// Once X is lined up, we reduce Y again to reach our target Y
		else if (targetLocation.y < currentLocation.y)
		{
			currentLocation.y -= 0.0005f;
		}
		else
		{
			isMoving = false;
		}
	}
	return isMoving;
}

void Disc::Create(float _offsetX, float _offsetY, vec4 _color)
{
	assert(bInitWasCalled);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	glGenBuffers(1, &vbos);
	glBindBuffer(GL_ARRAY_BUFFER, vbos);

	targetLocation = vec4(_offsetX, _offsetY, 0, 0);
	currentLocation = targetLocation;

	DiscQuad(1, 0, 3, 2);
	DiscQuad(2, 3, 7, 6);
	DiscQuad(3, 0, 4, 7);
	DiscQuad(6, 5, 1, 2);
	DiscQuad(4, 5, 6, 7);
	DiscQuad(5, 4, 0, 1);
	
	color = _color;

	// Define buffer size
	glBufferData(GL_ARRAY_BUFFER, (sizeof(discPoints) + sizeof(discNormals)),
		NULL, GL_STATIC_DRAW);
	float count = sizeof(discPoints) + sizeof(discNormals);
	// Define memory location for disc points and pass in array
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(discPoints), discPoints);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(discPoints), sizeof(discNormals), discNormals);
	
	// Create attrib values for use in shader language
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vNormal);

	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat),
		BUFFER_OFFSET(0));
	glBindAttribLocation(program, vPosition, "vPosition");
	
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat),
		BUFFER_OFFSET(sizeof(discPoints)*discVerticesCount) );
	glBindAttribLocation(program, vNormal, "vNormal");
	
	vColor = glGetUniformLocation(program, "vColor");
	vOffsetGPULocation = glGetUniformLocation(program, "vOffset");

	glBindVertexArray(0);
}

void Disc::Update()
{
}

void Disc::Draw(GLuint _modelView, mat4 _modelMat)
{
	glUseProgram(program);
	glUniformMatrix4fv(_modelView, 1, GL_TRUE, _modelMat);

	vColor = glGetUniformLocation(program, "vColor");
	vOffsetGPULocation = glGetUniformLocation(program, "vOffset");
	
	glUniform4fv(vOffsetGPULocation, 1, currentLocation);
	glUniform4fv(vColor, 1, color);
	
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, discVerticesCount);
	glBindVertexArray(0);
}