#pragma once
class Cradle {
public:
	GLuint program;		/* Identifier for the shader prgoram */
	GLuint vao;			/* Vertex array (Containor) object. This is the index of the VAO that will be the container foro ur buffer objects */

	GLuint colourmode;	/* Index of a uniform to switch the colour mode in the vertex shader  I've included this to show you how to pass in an unsigned integer into your vertex shader. */
	GLuint emitmode;

	/* Position and view globals */
	GLfloat angle_x, angle_inc_x, x, scale, z, y, vx, vy, vz;
	GLfloat angle_y, angle_inc_y, angle_z, angle_inc_z;
	GLuint drawmode;			// Defines drawing mode of sphere as points, lines or filled polygons


	GLfloat fLLenght, sWidth, fSLenght, fHeight, fWidth, pDistance;
	GLfloat bWithd, bY[5], bX[5];

	GLfloat light_x, light_y, light_z;

	/* Uniforms*/
	GLuint modelID, viewID, projectionID, lightposID, normalmatrixID;
	GLuint colourmodeID, emitmodeID;

	GLfloat aspect_ratio;		/* Aspect ratio of the window defined in the reshape callback*/

	GLuint swingMode;
	Square* square;
	Ball ball[5];

	void Init(GLWrapper *glw);
	void Display();
	void MoveBalls();
	static void Reshape(GLFWwindow* window, int w, int h);
	static void KeyCallback(GLFWwindow* window, int key, int s, int action, int mods);
};
