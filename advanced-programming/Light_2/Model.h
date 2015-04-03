#pragma once
#include <vector>
#include "Angel.h"
#include "GL/glut.h"

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

class Model
{
public:
	Model();
	virtual ~Model();

	virtual void Draw();
	virtual void Update();
	// Set the gl shader program for this model
	virtual void SetProgram(GLuint shaderName);
	// Clear this object from vertex and buffer arrays
	virtual void Destroy();
	// Gets the vertex array object for this model
	virtual GLuint GetVao()const;
	// Gets the vertex buffer object for this model
	virtual const GLuint GetVbos()const;

protected:
	color4 color;
	GLuint vao;
	GLuint program;
	GLuint vbos;
};
