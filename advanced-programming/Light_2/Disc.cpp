#include "Disc.h"

Disc::Disc() :
currentIndex(0),
bInitWasCalled(false)
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
	tempPoints[0] += currentLocation;

	tempPoints[1] = discVertices[_b];
	tempPoints[1] += currentLocation;

	tempPoints[2] = discVertices[_c];
	tempPoints[2] += currentLocation;

	tempPoints[3] = discVertices[_d];
	tempPoints[3] += currentLocation;

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

void Disc::SetTargetOffsets(float _xTarget, float _yTarget)
{
	targetLocation = vec4(_xTarget, _yTarget, 0, 0);
}

bool Disc::UpdatePositions()
{
	bool moreUpdates = false;
	// Check the discrepency between current offset and target offset values
	if (targetLocation.y - currentLocation.y > 0.05f)
	{
		currentLocation.y += 0.01f;
		moreUpdates = true;
	}
	else if (targetLocation.y - currentLocation.y < -0.05f)
	{
		currentLocation.y -= 0.01f;
		moreUpdates = true;
	}
	if (targetLocation.x - currentLocation.x > 0.05f)
	{
		currentLocation.x += 0.01f;
		moreUpdates = true;
	}
	else if (targetLocation.x - currentLocation.x < -0.05f)
	{
		currentLocation.x -= 0.01f;
		moreUpdates = true;
	}

	return moreUpdates;
}

void Disc::Create(float _offsetX, float _offsetY)
{
	assert(bInitWasCalled);
	// Create and initialize a buffer object
	glGenBuffers(1, &vbos);
	glBindBuffer(GL_ARRAY_BUFFER, vbos);
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	SetTargetOffsets(_offsetX, _offsetY);
	currentLocation = targetLocation;

	DiscQuad(1, 0, 3, 2);
	DiscQuad(2, 3, 7, 6);
	DiscQuad(3, 0, 4, 7);
	DiscQuad(6, 5, 1, 2);
	DiscQuad(4, 5, 6, 7);
	DiscQuad(5, 4, 0, 1);
	
	color = vec4(0.2f, _offsetY, _offsetX, 1.0f);

	// Define buffer size
	glBufferData(GL_ARRAY_BUFFER, (sizeof(discPoints) + sizeof(discNormals) + sizeof(color) + sizeof(currentLocation)),
		NULL, GL_STATIC_DRAW);

	// Define memory location for disc points and pass in array
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(discPoints), discPoints);
	
	// Define memory location for disc normals and pass in array
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(discPoints),
		sizeof(discNormals), discNormals);
	
	// Define memory location for disc color and pass in vec
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(discPoints) + sizeof(discNormals),
		sizeof(color), color);
	
	// Create attrib values for use in shader language
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vNormal);
	glEnableVertexAttribArray(vColor);

	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat),
		BUFFER_OFFSET(0));
	glBindAttribLocation(program, vPosition, "vPosition");
	
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat),
		BUFFER_OFFSET(sizeof(discPoints)*discVerticesCount) );
	glBindAttribLocation(program, vNormal, "vNormal");
	
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat),
		BUFFER_OFFSET(sizeof(discPoints)*discVerticesCount + sizeof(discNormals)*discVerticesCount) );
	glBindAttribLocation(program, vColor, "vColor");
	
	vOffsetGPULocation = glGetUniformLocation(program, "vOffset");
}

void Disc::Update()
{
	currentLocation.x += 0.01f;
	// Define memory location for position off set and pass in vec
	glUniform4f(vOffsetGPULocation, currentLocation.x,currentLocation.y, currentLocation.z, currentLocation.w);
}

void Disc::Draw()
{
	glUseProgram(program);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, discVerticesCount);
}