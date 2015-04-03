#include "Disc.h"

Disc::Disc() :
currentIndex(0),
bInitWasCalled(false)
{
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

void Disc::DiscQuad(int _a, int _b, int _c, int _d, float _offsetX, float _offsetY)
{
	// Initialize temporary vectors along the quad's edge to
	// compute its face normal 
	point4 tempPoints[4];
	tempPoints[0] = discVertices[_a];
	tempPoints[0].x += _offsetX;
	tempPoints[0].y += _offsetY;

	tempPoints[1] = discVertices[_b];
	tempPoints[1].x += _offsetX;
	tempPoints[1].y += _offsetY;

	tempPoints[2] = discVertices[_c];
	tempPoints[2].x += _offsetX;
	tempPoints[2].y += _offsetY;

	tempPoints[3] = discVertices[_d];
	tempPoints[3].x += _offsetX;
	tempPoints[3].y += _offsetY;

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
void Disc::Create(float _offsetX, float _offsetY)
{
	assert(bInitWasCalled);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	glGenBuffers(1, &vbos);
	glBindBuffer(GL_ARRAY_BUFFER, vbos);
	
	DiscQuad(1, 0, 3, 2, _offsetX, _offsetY);
	DiscQuad(2, 3, 7, 6, _offsetX, _offsetY);
	DiscQuad(3, 0, 4, 7, _offsetX, _offsetY);
	DiscQuad(6, 5, 1, 2, _offsetX, _offsetY);
	DiscQuad(4, 5, 6, 7, _offsetX, _offsetY);
	DiscQuad(5, 4, 0, 1, _offsetX, _offsetY);

	color = vec4(0.2f, _offsetY, _offsetX, 1.0f);

	// Define buffer size
	glBufferData(GL_ARRAY_BUFFER, (sizeof(discPoints)+sizeof(discNormals)+sizeof(color)),
		NULL, GL_STATIC_DRAW);
	// Define memory location for disc points and pass in array
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(discPoints), discPoints);
	// Define memory location for disc normals and pass in array
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(discPoints),
		sizeof(discNormals), discNormals);
	// Define memory location for disc color and pass in vec
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(discPoints)+sizeof(discNormals),
		sizeof(color), color);

	// Create attrib values for use in shader language
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(discPoints)));

	GLuint baseColor = glGetAttribLocation(program, "baseColor");
	glEnableVertexAttribArray(baseColor);
	glVertexAttribPointer(baseColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(discPoints) + sizeof(discNormals)) );
}

void Disc::Update()
{}

void Disc::Draw()
{
	glUseProgram(program);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, discVerticesCount);
}