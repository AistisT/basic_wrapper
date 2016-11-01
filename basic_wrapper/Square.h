#pragma once
class Square
{

public:
	GLuint positionBufferObject, colourObject, normalsBufferObject;

	void CreateSquare();
	void DisplaySquare(glm::mat4& model, glm::mat4& View, GLuint modelID, GLuint normalmatrixID,GLfloat x, GLfloat y, GLfloat z);
};

