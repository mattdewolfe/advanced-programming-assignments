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
	
	vPosition = 0;
	vNormal = 1;
	vColor = 2;
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

void Tower::TowerQuad(int _a, int _b, int _c, int _d)
{
	// Initialize temporary vectors along the quad's edge to
	// compute its face normal 
	point4 tempPoints[4];
	tempPoints[0] = towerVertices[_a];

	tempPoints[1] = towerVertices[_b];

	tempPoints[2] = towerVertices[_c];

	tempPoints[3] = towerVertices[_d];

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
	// Create and initialize a buffer object
	glGenBuffers(1, &vbos);
	glBindBuffer(GL_ARRAY_BUFFER, vbos);
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	TowerQuad(1, 0, 3, 2);
	TowerQuad(2, 3, 7, 6);
	TowerQuad(3, 0, 4, 7);
	TowerQuad(6, 5, 1, 2);
	TowerQuad(4, 5, 6, 7);
	TowerQuad(5, 4, 0, 1);

	targetLocation = vec4(_offset, 0, 0, 0);
	currentLocation = vec4(_offset, 0, 0, 0);
	color = vec4(0.7f, 0.25f, 0.7f, 1.0f);

	// Define buffer size
	glBufferData(GL_ARRAY_BUFFER, sizeof(towerPoints) + sizeof(towerNormals) + sizeof(color) + sizeof(currentLocation),
		NULL, GL_STATIC_DRAW);
	// Define memory location tower points and pass in array
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(towerPoints), towerPoints);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(towerPoints), sizeof(towerNormals), towerNormals);

	// Create attrib values for use in shader language
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vNormal);

	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat),
		BUFFER_OFFSET(0));
	glBindAttribLocation(program, vPosition, "vPosition");
	
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat),
		BUFFER_OFFSET(sizeof(towerPoints)*towerVerticesCount) );
	glBindAttribLocation(program, vNormal, "vNormal");
	
	vColor = glGetUniformLocation(program, "vColor");
	vOffsetGPULocation = glGetUniformLocation(program, "vOffset");
}

void Tower::Update()
{
	Draw();
}

void Tower::Draw()
{
	vColor = glGetUniformLocation(program, "vColor");
	vOffsetGPULocation = glGetUniformLocation(program, "vOffset");
	glUseProgram(program);
	glUniform4fv(vOffsetGPULocation, 1, currentLocation);
	glUniform4fv(vColor, 1, color);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, towerVerticesCount);
}