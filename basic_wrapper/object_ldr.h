#pragma once
/* object_ldr.h
   Example class to show the start of an .obj mesh obkect file
   loader
   Iain Martin November 2014
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class object_ldr
{
public:
	object_ldr();


	void load_obj(const char* filename);
	void drawObject(glm::mat4& View, GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz, GLfloat vx, GLfloat vy, GLfloat vz);
	void createObject();
	void smoothNormals();
	void Init(GLuint modelID, GLuint normalmatrixID);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<GLushort> elements;
	glm::mat4 model;
	glm::mat3 normalmatrix;
	GLuint modelID, normalmatrixID;

	GLuint vbo_mesh_vertices;
	GLuint vbo_mesh_normals;
	GLuint ibo_mesh_elements;
	GLuint attribute_v_coord;
	GLuint attribute_v_colours;
	GLuint attribute_v_normal;
};

