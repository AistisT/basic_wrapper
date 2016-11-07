
/* Link to static libraries */
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>
 /* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Square.h"
#include "Ball.h"
#include "Cylinder.h"


GLuint program;		/* Identifier for the shader prgoram */
GLuint vao;			/* Vertex array (Containor) object. This is the index of the VAO that will be the container foro ur buffer objects */

GLuint colourmode;	/* Index of a uniform to switch the colour mode in the vertex shader  I've included this to show you how to pass in an unsigned integer into your vertex shader. */
GLuint emitmode;

/* Position and view globals */
GLfloat  vx, vy, vz;

bool third = false;
const GLuint bNr = 5, pair = 2;
const GLfloat fLLenght = 1.5f, sWidth = 0.3f, fSLenght = 0.85f, fHeight = 1.2f, fWidth = 1.f, pDistance = 0.675f, swingS = 0.135;
const GLfloat bSpeed = 0.12f, bXR = 0.014;
GLfloat bWithd, bY[bNr], bX[bNr], bArc[bNr];
GLuint bDir[5];
GLfloat cY[bNr][pair], cX[bNr][pair], cZ[bNr][pair], cRY[bNr][pair], cRX[bNr][pair], cRZ[bNr][pair];
GLfloat light_x, light_y, light_z;

/* Uniforms*/
GLuint modelID, viewID, projectionID, lightposID, normalmatrixID;
GLuint colourmodeID, emitmodeID;

GLfloat aspect_ratio;		/* Aspect ratio of the window defined in the reshape callback*/

GLuint swingMode, swing;
Square* square;
Ball ball[bNr];
Cylinder cylinder[bNr][pair];
void MoveBalls();
void Swing1(GLuint ball, GLfloat s, GLfloat d, GLuint t, GLuint nBall, GLfloat bRot, GLfloat bRot2);
void Swing2(GLuint ball, GLfloat s, GLfloat d, GLuint t, GLuint nBall, GLfloat bRot, GLfloat bRot2);

/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper *glw)
{
	/* Set the object transformation controls to their initial values */

	vx = 20; vy = 0, vz = 0.f;

	aspect_ratio = 1.3333f;
	colourmode = 0; emitmode = 0;

	bWithd = .225f / 2;

	light_z = -fWidth / 2;
	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	square = new Square();
	square->DefineSquare();


	/* Load and build the vertex and fragment shaders */
	try
	{
		program = glw->LoadShader("poslight.vert", "poslight.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	/* Define uniforms to send to vertex shader */
	modelID = glGetUniformLocation(program, "model");
	colourmodeID = glGetUniformLocation(program, "colourmode");
	emitmodeID = glGetUniformLocation(program, "emitmode");
	viewID = glGetUniformLocation(program, "view");
	projectionID = glGetUniformLocation(program, "projection");
	lightposID = glGetUniformLocation(program, "lightpos");
	normalmatrixID = glGetUniformLocation(program, "normalmatrix");

	GLfloat distance = -0.675f + bWithd * 2;
	for (int i = 0; i < bNr; i++) {
		ball[i] = Ball();
		ball[i].Init(modelID, normalmatrixID);
		bY[i] = .5;
		bX[i] = distance;
		distance += bWithd * 2;
		bDir[i] = 0;
		for (int j = 0; j < pair; j++) {
			cylinder[i][j] = Cylinder();
			cylinder[i][j].Init(modelID, normalmatrixID);
			cX[i][j] = bX[i];
			cY[i][j] = .5;
			if (j == 0)
			{
				cZ[i][j] = 0;
				cRX[i][j] = 29;
			}
			else
			{
				cZ[i][j] = -fWidth;
				cRX[i][j] = -29;
			}
		}
	}
}

/* Called to update the display. Note that this function is called in the event loop in the wrapper
   class because we registered display as a callback function */
void display()
{
	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	/* Make the compiled shader program current */
	glUseProgram(program);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(30.0f, aspect_ratio, 0.1f, 100.0f);

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 4), // Camera is at (0,0,4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Apply rotations to the view position
	View = glm::rotate(View, vx, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	View = glm::rotate(View, vy, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	View = glm::rotate(View, vz, glm::vec3(0, 0, 1));

	// Define the light position and transform by the view matrix
	glm::vec4 lightpos = View *  glm::vec4(light_x, light_y, light_z, 1.0);

	glUniform1ui(colourmodeID, colourmode);
	glUniform1ui(emitmodeID, emitmode);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));

	square->Init(modelID, normalmatrixID);

	//view, x, y ,z positions, x, y,z, scale, x,y,z, rotation
	// frame
	square->DisplaySquare(View, 0, .4, 0, sWidth, fLLenght, sWidth, 0, 0, 90);
	square->DisplaySquare(View, 0, .4, -fWidth, sWidth, fLLenght, sWidth, 0, 0, 90);

	square->DisplaySquare(View, pDistance, -.27, 0, sWidth, fHeight, sWidth, 0, 0, 0);
	square->DisplaySquare(View, -pDistance, -0.27, 0, sWidth, fHeight, sWidth, 0, 0, 0);

	square->DisplaySquare(View, pDistance, -.27, -fWidth, sWidth, fHeight, sWidth, 0, 0, 0);
	square->DisplaySquare(View, -pDistance, -0.27, -fWidth, sWidth, fHeight, sWidth, 0, 0, 0);

	square->DisplaySquare(View, pDistance, -.795, -fWidth / 2, sWidth, fSLenght, sWidth, 90, 0, 0);
	square->DisplaySquare(View, -pDistance, -.795, -fWidth / 2, sWidth, fSLenght, sWidth, 90, 0, 0);


	for (int i = 0; i < bNr; i++) {
		ball[i].drawSphere(View, bX[i], bY[i], -fWidth / 2, bWithd, bArc[i], false);
		for (int j = 0; j < pair; j++) {
			cylinder[i][j].DrawCylinder(View, cX[i][j], cY[i][j], cZ[i][j], .1, cRX[i][j], cRY[i][j], cRZ[i][j]);
		}
	}

	/* Draw our lightposition sphere */
	emitmode = 1;
	glUniform1ui(emitmodeID, emitmode);
	ball->drawSphere(View, light_x, light_y, light_z, 0.05, bArc[3], true);
	emitmode = 0;
	glDisableVertexAttribArray(0);
	glUseProgram(0);

	MoveBalls();
}

void MoveBalls() {
	for (int i = 1; i <= swingMode; i++) {
		if (bDir[i-1] == 1 &&  !third) {
			Swing1(i - 1, -swingS,  -44.955, 2, i - 1, bXR, -bXR);
		}
		else if  (bDir[i-1] == 2 ) {
			Swing2(i - 1, swingS,  0, 1, bNr - i, -bXR, bXR);
			third = true;
		}
		else if (bDir[bNr-i] == 1 ) {
			Swing2(bNr - i, swingS,  44.955, 2, bNr - i, bXR, -bXR);
		}
		else if (bDir[bNr-i] == 2 ) {
			Swing1(bNr - i, -swingS,  0, 1, i-1, -bXR, bXR);
			third = false;
		}
	}

}

void Swing1(GLuint ball, GLfloat s, GLfloat d, GLuint t, GLuint nBall, GLfloat bRot, GLfloat bRot2) {

	bArc[ball] += s;

	for (int j = 0; j < pair; j++) {
		cRZ[ball][j] += -bSpeed;
		if (cZ[ball][j] == 0)
			cRX[ball][j] += bRot;
		else
			cRX[ball][j] += bRot2;
	}
	if (bArc[ball] <= d) {
		//bArc[ball] = d;
		bDir[ball] = 0;
		bDir[nBall] = t;
		if (swing == 0 && ball != nBall) {
			swingMode = 0;
			bDir[nBall] = 0;
		}
	}
}
void Swing2(GLuint ball, GLfloat s, GLfloat d, GLuint t, GLuint nBall, GLfloat bRot, GLfloat bRot2) {
	bArc[ball] += s;

	for (int j = 0; j < pair; j++) {
		cRZ[ball][j] += bSpeed;
		if (cZ[ball][j] == 0)
			cRX[ball][j] += bRot;
		else
			cRX[ball][j] += bRot2;
	}
	if (bArc[ball] >= d) {
		//bArc[ball] = d;
		bDir[ball] = 0;
		bDir[nBall] = t;
		if (swing == 0 && ball != nBall) {
			swingMode = 0;
			bDir[nBall] = 0;
		}
	}
}

/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	aspect_ratio = ((float)w / 640.f*4.f) / ((float)h / 480.f*3.f);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
	/* Enable this call if you want to disable key responses to a held down key*/
	//if (action != GLFW_PRESS) return;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);


	if (key == '1' && swingMode == 0) {
		swing = 1;
		swingMode = 1;
		bDir[0] = 1;
	}
	if (key == '2' && swingMode ==0)
	{
		swing = 1;
		swingMode = 2;
		bDir[0] = 1;
		bDir[1] = 1;
	}
	if (key == '3' && swingMode == 0)
	{
		swing = 1;
		swingMode = 3;
		bDir[0] = 1;
		bDir[1] = 1;
		bDir[2] = 1;
	}
	if (key == '4' && swingMode == 0)
	{
		swing = 1;
		swingMode = 3;
		bDir[0] = 1;
		bDir[1] = 1;
		bDir[2] = 1;
		bDir[3] = 1;
	}
	if (key == '0') {
		swing = 0;
	}
	if (key == 'F') light_x -= 0.05f;
	if (key == 'H') light_x += 0.05f;
	if (key == 'G') light_y -= 0.05f;
	if (key == 'T') light_y += 0.05f;
	if (key == 'R') light_z -= 0.05f;
	if (key == 'Y') light_z += 0.05f;
	if (key == 'S') vx -= 1.f;
	if (key == 'W') vx += 1.f;
	if (key == 'D') vy -= 1.f;
	if (key == 'A') vy += 1.f;
	if (key == 'Q') vz -= 1.f;
	if (key == 'E') vz += 1.f;
}

/* Entry point of program */
int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(1024, 768, "Newton's Cradle");;

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);

	init(glw);

	glw->eventLoop();
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	delete(glw);
	return 0;
}