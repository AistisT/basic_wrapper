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


	void Square::DefineSquare(){
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

	void Square::Init( GLuint modelID, GLuint normalmatrixID){
		this->modelID = modelID;
		this->normalmatrixID = normalmatrixID;
	}
	void Square::DisplaySquare(glm::mat4& View, GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz, GLfloat vx, GLfloat vy, GLfloat vz){
		model = glm::mat4(1.0f);
		// Define the model transformations for the cube
		model = glm::translate(model, glm::vec3(x, y, z));
		model = glm::scale(model, glm::vec3(sx, sy, sz));//scale equally in all axis
		model = glm::rotate(model, vx, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
		model = glm::rotate(model, vy, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
		model = glm::rotate(model, vz, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis

		// Define the normal matrix
		normalmatrix = glm::transpose(glm::inverse(glm::mat3(View * model)));


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

		glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

	}
