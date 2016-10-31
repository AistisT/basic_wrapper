#pragma once
class Square
{

public:
	GLuint positionBufferObject, colourObject, normalsBufferObject;
	/* Position and view globals */

	GLfloat angle_x, x, scale, z, y, angle_y, angle_z;
	void CreateSquare();
	void DisplaySquare();
};

