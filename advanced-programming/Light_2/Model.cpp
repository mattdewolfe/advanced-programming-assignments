#include "Model.h"


Model::Model()
{
}


Model::~Model()
{
	Destroy();
}

void Model::SetProgram(GLuint program)
{
	this->program = program;
}

GLuint Model::GetVao() const
{
	return vao;
}

const GLuint Model::GetVbos() const
{
	return vbos;
}

void Model::Destroy()
{
	glDeleteVertexArrays(1, &vao);
}

// Abstract functions
void Model::Draw(){}
void Model::Update(){}