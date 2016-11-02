#pragma once
class Square
{

public:
	GLuint positionBufferObject, colourObject, normalsBufferObject;
	glm::mat4 model;
	glm::mat3 normalmatrix;
	GLuint modelID, normalmatrixID;

	void DefineSquare();
	void DisplaySquare(glm::mat4& View, GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz, GLfloat vx, GLfloat vy, GLfloat vz);
	void Init(GLuint modelID, GLuint normalmatrixID);
};

