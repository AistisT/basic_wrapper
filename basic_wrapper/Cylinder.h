#pragma once
class Cylinder
{
public:
	GLuint sphereBufferObject, sphereNormals, sphereColours, elementbuffer;
	GLuint numlats, numlongs;
	GLuint numspherevertices;
	GLuint modelID, normalmatrixID;

	void Cylinder::Init(GLuint modelID, GLuint normalmatrixID);
	GLuint Cylinder::MakeCylinderVBO(GLuint numlats, GLuint numlongs);
	void Cylinder::MakeUnitCylinder(GLfloat *pVertices, GLuint numlats, GLuint numlongs);
	void Cylinder::DrawCylinder(glm::mat4& View, GLfloat x, GLfloat y, GLfloat z, GLfloat scale, GLfloat vx, GLfloat vy, GLfloat vz);
};

