#pragma once
class Ball
{
public:
	GLuint sphereBufferObject, sphereNormals, sphereColours,elementbuffer;
	GLuint numlats, numlongs;
	GLuint numspherevertices;
	GLuint modelID, normalmatrixID;

	void makeUnitSphere(GLfloat *pVertices, GLuint numlats, GLuint numlongs);
	GLuint makeSphereVBO(GLuint numlats, GLuint numlongs);
	void drawSphere(glm::mat4& View, GLfloat x, GLfloat y, GLfloat z, GLfloat scale, GLfloat arc, bool light);
	void Init(GLuint modelID, GLuint normalmatrixID);
};

