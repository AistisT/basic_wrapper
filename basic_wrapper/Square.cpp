#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>

/* Include GLM core and matrix extensions*/

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "Square.h"




	void Square::CreateSquare(){
		GLuint triangles = 36;
		/* Define vertices for a cube in 12 triangles */
		GLfloat vertexPositions[] =
		{
			-0.25f, 0.50f, -0.25f,
			-0.25f, -0.50f, -0.25f,
			0.25f, -0.50f, -0.25f,

			0.25f, -0.50f, -0.25f,
			0.25f, 0.50f, -0.25f,
			-0.25f, 0.50f, -0.25f,

			0.25f, -0.50f, -0.25f,
			0.25f, -0.50f, 0.25f,
			0.25f, 0.50f, -0.25f,

			0.25f, -0.50f, 0.25f,
			0.25f, 0.50f, 0.25f,
			0.25f, 0.50f, -0.25f,

			0.25f, -0.50f, 0.25f,
			-0.25f, -0.50f, 0.25f,
			0.25f, 0.50f, 0.25f,

			-0.25f, -0.50f, 0.25f,
			-0.25f, 0.50f, 0.25f,
			0.25f, 0.50f, 0.25f,

			-0.25f, -0.5f, 0.25f,
			-0.25f, -0.5f, -0.25f,
			-0.25f, 0.5f, 0.25f,

			-0.25f, -0.5f, -0.25f,
			-0.25f, 0.5f, -0.25f,
			-0.25f, 0.5f, 0.25f,

			-0.25f, -0.5f, 0.25f,
			0.25f, -0.5f, 0.25f,
			0.25f, -0.5f, -0.25f,

			0.25f, -0.5f, -0.25f,
			-0.25f, -0.5f, -0.25f,
			-0.25f, -0.5f, 0.25f,

			-0.25f, 0.5f, -0.25f,
			0.25f, 0.5f, -0.25f,
			0.25f, 0.5f, 0.25f,

			0.25f, 0.5f, 0.25f,
			-0.25f, 0.5f, 0.25f,
			-0.25f, 0.5f, -0.25f,




		};

		/* Manually specified colours for our cube */
		
		float vertexColours[sizeof(vertexPositions)];
		for (int i = 0; i < sizeof(vertexPositions); i++)
		{
			vertexColours[i] = 1;
		}

		/* Manually specified normals for our cube */
		GLfloat normals[] =
		{
			0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
			0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
			1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
			1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
			0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
			0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
			-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
			-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
			0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
			0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
			0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
			0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0
		};

		/* Create the vertex buffer for the cube */
		glGenBuffers(1, &positionBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/* Create the colours buffer for the cube */
		glGenBuffers(1, &colourObject);
		glBindBuffer(GL_ARRAY_BUFFER, colourObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColours), vertexColours, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/* Create the normals  buffer for the cube */
		glGenBuffers(1, &normalsBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), normals, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Square::DisplaySquare(){

		/* Bind cube vertices. Note that this is in attribute index 0 */
		glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		/* Bind cube colours. Note that this is in attribute index 1 */
		glBindBuffer(GL_ARRAY_BUFFER, colourObject);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

		/* Bind cube normals. Note that this is in attribute index 2 */
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	
	}
