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
#include "object_ldr.h"

GLuint program;		/* Identifier for the shader prgoram */
GLuint vao;			/* Vertex array (Containor) object. This is the index of the VAO that will be the container foro ur buffer objects */

GLuint emitmode;

/* Position and view globals */
GLfloat  vx, vy, vz;

// assignment 2 vars
object_ldr monkey; // Create a global object_ldr object





// light variables
GLfloat light_x, light_y = .7f, light_z= -.5f;;

/* Uniforms*/
GLuint modelID, viewID, projectionID, lightposID, normalmatrixID;
GLuint colourmodeID, emitmodeID;

GLfloat aspect_ratio;		/* Aspect ratio of the window defined in the reshape callback*/


Square square;
Ball sun;

/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper *glw)
{
	/* Set the object transformation controls to their initial values */
	vx = 20; vy = 0, vz = 0.f;
	aspect_ratio = 1.3333f; emitmode = 0;

	///* Load and create our monkey object in an init() function */
	monkey.load_obj("monkey.obj");
	monkey.smoothNormals();
	monkey.createObject();


	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	square =  Square();
	square.DefineSquare();

	sun =  Ball();


	/* Load and build the vertex and fragment shaders */
	try
	{
		program = glw->LoadShader("cradle.vert", "cradle.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	/* Define uniforms to send to vertex shader */
	modelID = glGetUniformLocation(program, "model");
	emitmodeID = glGetUniformLocation(program, "emitmode");
	viewID = glGetUniformLocation(program, "view");
	projectionID = glGetUniformLocation(program, "projection");
	lightposID = glGetUniformLocation(program, "lightpos");
	normalmatrixID = glGetUniformLocation(program, "normalmatrix");

	sun.Init(modelID, normalmatrixID);
	monkey.Init(modelID, normalmatrixID);
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
		glm::vec3(0, 0, 8), // Camera is at (0,0,4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Apply rotations to the view position
	View = glm::rotate(View, vx, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	View = glm::rotate(View, vy, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	View = glm::rotate(View, vz, glm::vec3(0, 0, 1));

	// Define the light position and transform by the view matrix
	glm::vec4 lightpos = View *  glm::vec4(light_x, light_y, light_z, 1.0);

	glUniform1ui(emitmodeID, emitmode);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));

	square.Init(modelID, normalmatrixID);

	//view, x, y ,z positions, x, y,z, scale, x,y,z, rotation
	// frame construction
	//square.DisplaySquare(View, 0, .4, 0, 1, 1, 1, 0, 0, 90);

	/* Draw our object in the display() function */
	monkey.drawObject(View, .5,0,0,1,1,1,0,0,0);



	/* Draw our lightposition sphere */
	emitmode = 1;
	glUniform1ui(emitmodeID, emitmode);
	sun.drawSphere(View, light_x, light_y, light_z, .1, 0, true);
	emitmode = 0;
	glDisableVertexAttribArray(0);
	glUseProgram(0);

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