#pragma once
#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include "..\..\dependencies\glm\glm\glm.hpp"
#include "..\..\dependencies\glm\glm\gtc\matrix_transform.hpp"
#include "..\..\glm\glm\gtc\type_ptr.hpp"

//stb_image
#include "..\..\Common\include\stb_image.h"

// Nossa classe que armazena as infos dos shaders
#include "..\..\Common\include\Shader.h"

using namespace std;
class Object
{
	Object() {}
	~Object();

	void initialize();
	void update();
	void draw();

	inline void setTexture(GLuint texID) { this->texID = texID; }
	inline void setShader(Shader* shader) { this->shader = shader; }
	inline void setPosition(glm::vec3 position) { this->position = position; }//ver tbm
	inline void setDimensions(glm::vec3 scale) { this->scale = scale; }
	inline void setAngle(float angle) { this->angle = angle; }

protected:
	void setRotation(float angle, glm::vec3 axis, bool reset) {} // ver a propriedade completa
	void setTranslation(glm::vec3 displacements,bool reset) {}
	void setScale(glm::vec3 scaleFactors, bool reset) {}

	GLuint VAO;
	glm::mat4 transform;
	GLuint texID;
	Shader* shader;
	glm::mat3 position, scale;
	float angle;
};

