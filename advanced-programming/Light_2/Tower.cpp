#include "Tower.h"

// Vertices of the towers
static point4 const towerVertices[8] = {
	point4(-0.05, -0.4, 0.05, 1.0),
	point4(-0.05, 0.4, 0.05, 1.0),
	point4(0.05, 0.4, 0.05, 1.0),
	point4(0.05, -0.4, 0.05, 1.0),
	point4(-0.05, -0.4, -0.05, 1.0),
	point4(-0.05, 0.4, -0.05, 1.0),
	point4(0.05, 0.4, -0.05, 1.0),
	point4(0.05, -0.4, -0.05, 1.0)
};

Tower::Tower():
currentIndex(0),
topDiscIndex(-1)
{
	for (int i = 0; i < 6; i++)
	{
		discs[i] = 0;
	}
}

Tower::~Tower()
{}

void Tower::PrintTowerLocations()
{
	for (int i = 0; i <= topDiscIndex; i++)
	{
		printf(" %d ", discs[i]);
	}
}

bool Tower::PlaceDiscOnTower(int _discNumber)
{
	// If we are at index -1, there are no discs on this tower yet
	if (topDiscIndex == -1)
	{
		discs[0] = _discNumber;
		topDiscIndex = 0;
	}
	// If the top disc has a larger index then the disc the user wants
	// to place here, then they cannot as that would be a larger disc on top
	// of a smaller disc
	else if (discs[topDiscIndex] > _discNumber)
	{
		return false;
	}
	// If the top index is 5, this tower is full
	else if (topDiscIndex == 5)
	{
		return false;
	}
	// Otherwise, the disc can go here so increment top index tracker and place disc here 
	else
	{
		topDiscIndex++;
		discs[topDiscIndex] = _discNumber;
	}
	return true;
}

void Tower::ClearDiscs()
{
	while (topDiscIndex != -1)
	{
		RemoveTopDisc();
	}
}

void Tower::RemoveTopDisc()
{
	// Ensure the tower is not empty
	if (topDiscIndex != -1)
	{
		// Then set the top value to -1
		discs[topDiscIndex] = -1;
		// And decrement top index tracker
		topDiscIndex--;
	}
}

void Tower::FillDiscArrayToIndex(int _index)
{
	for (int i = 0; i < _index; i++)
	{
		discs[i] = i;
		topDiscIndex = i;
	}
}

void Tower::TowerQuad(int _a, int _b, int _c, int _d, float _offset)
{
	// Initialize temporary vectors along the quad's edge to
	// compute its face normal 
	point4 tempPoints[4];
	tempPoints[0] = towerVertices[_a];
	tempPoints[0].x += _offset;

	tempPoints[1] = towerVertices[_b];
	tempPoints[1].x += _offset;

	tempPoints[2] = towerVertices[_c];
	tempPoints[2].x += _offset;

	tempPoints[3] = towerVertices[_d];
	tempPoints[3].x += _offset;

	vec4 u = towerVertices[1] - towerVertices[0];
	vec4 v = towerVertices[2] - towerVertices[1];

	vec3 normal = normalize(cross(u, v));

	towerNormals[currentIndex] = normal; towerPoints[currentIndex] = tempPoints[0]; currentIndex++;
	towerNormals[currentIndex] = normal; towerPoints[currentIndex] = tempPoints[1]; currentIndex++;
	towerNormals[currentIndex] = normal; towerPoints[currentIndex] = tempPoints[2]; currentIndex++;
	towerNormals[currentIndex] = normal; towerPoints[currentIndex] = tempPoints[0]; currentIndex++;
	towerNormals[currentIndex] = normal; towerPoints[currentIndex] = tempPoints[2]; currentIndex++;
	towerNormals[currentIndex] = normal; towerPoints[currentIndex] = tempPoints[3]; currentIndex++;
}

void Tower::Create(float _offset)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	glGenBuffers(1, &vbos);
	glBindBuffer(GL_ARRAY_BUFFER, vbos);
	
	TowerQuad(1, 0, 3, 2, _offset);
	TowerQuad(2, 3, 7, 6, _offset);
	TowerQuad(3, 0, 4, 7, _offset);
	TowerQuad(6, 5, 1, 2, _offset);
	TowerQuad(4, 5, 6, 7, _offset);
	TowerQuad(5, 4, 0, 1, _offset);

	targetLocation = vec4(0, 0, 0, 0);
	currentLocation = vec4(0, 0, 0, 0);
	color = vec4(1.0f, 0, 0, 1.0f);

	// Define buffer size
	glBufferData(GL_ARRAY_BUFFER, (sizeof(towerPoints)+sizeof(towerNormals)+sizeof(color)+sizeof(currentLocation)),
		NULL, GL_STATIC_DRAW);
	// Define memory location tower points and pass in array
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(towerPoints), towerPoints);
	// Define memory location tower normals and pass in array
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(towerPoints),
		sizeof(towerNormals), towerNormals);
	// Define memory location for color and pass in vec
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(towerPoints) + sizeof(towerNormals)),
		sizeof(color), color);
	// Define memory location for position off set and pass in vec
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(towerPoints) + sizeof(towerNormals) + sizeof(color) ),
		sizeof(currentLocation), currentLocation);

	// Create attrib values for use in shader language
	GLuint vPosition = 0;
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);
	
	GLuint vNormal = 1;
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(towerPoints)));
	glEnableVertexAttribArray(vNormal);
	
	GLuint vColor = 2;
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(towerPoints)+sizeof(towerNormals)) );
	glEnableVertexAttribArray(vColor);
	
	GLuint vOffset = 3;
	glVertexAttribPointer(vOffset, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(towerPoints)+sizeof(towerNormals)+sizeof(color)) );
	glEnableVertexAttribArray(vOffset);
	
}

void Tower::Update()
{}

void Tower::Draw()
{
	glUseProgram(program);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, towerVerticesCount);
}