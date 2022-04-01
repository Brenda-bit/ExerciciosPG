/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 05/03/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <cmath> //Biblioteca math
const float Pi = 3.14159;
// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
//int setupShader();
int createCircle(float radius, int nPoints);

int createStar(float radius, int nPoints);
// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;


// Função MAIN
int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! Brenda", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	const GLubyte* renderer = glGetString(GL_RENDERER); 
	const GLubyte* version = glGetString(GL_VERSION); 
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	Shader shader("../Shaders/Hello.vs", "../Shaders/Hello.fs");

	//Circulo (mantendo o valor do GL_TRIANGLES para 102)
	/*GLuint VAO = createCircle(0.5, 100);*/
	
	//Octagno (mantendo o valor do GL_TRIANGLES para 102)
	/*GLuint VAO = createCircle(0.5, 8);*/
	
	//Hexagono (mantendo o valor do GL_TRIANGLES para 102)
	/*GLuint VAO = createCircle(0.5, 6);*/

	//Estrela
	/*GLuint VAO = createStar(0.5, 10);*/
	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);
	
	glUseProgram(shader.ID);
	

	while (!glfwWindowShouldClose(window))
	{
		
		glfwPollEvents();

		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		//Círculo
		glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 102);
		glBindVertexArray(0);
		
		//Pacman (mantendo o createCircle(0,100))
		/*glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 86);
		glBindVertexArray(0);*/

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int createCircle(float radius, int nPoints)
{
	float* vertices; // é interessante estar em ponteiro pra justamente fazer outras formas além do círculo
	float totalSize = (nPoints + 2) * 3; //*3 pq o conjunto de triangulos retang forma um círculo, mais um pq digamos q queremos 100 pontos teremos q adicionar +1 pro raio
	//primeiro vértice, centro do círculo (miolo do círculo pensando na ideia do triangulo retang)
	vertices = new float[totalSize];
	vertices[0] = 0.0; //x
	vertices[1] = 0.0; //y
	vertices[2] = 0.0; //z


	float angle = 0.0;
	float slice = 2 * Pi / (float)nPoints; // nao incluindo o ponto central
	int i = 3;
	while (i < totalSize) {
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		float z = 0.0;

		vertices[i] = x;
		vertices[i + 1] = y;
		vertices[i + 2] = z;

		i += 3;
		angle += slice;
	}
	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, totalSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
};
int createStar(float radius, int nPoints)
{
	float* vertices; 
	float totalSize = (nPoints + 2) * 3;
	vertices = new float[totalSize];
	vertices[0] = 0.0; //x
	vertices[1] = 0.0; //y
	vertices[2] = 0.0; //z

	float angle = 0.0;
	float slice = 2 * Pi / (float)nPoints; 
	int i = 3;
	while (i < totalSize)
	{
		if (i % 2 == 0)
		{
			float x = radius * cos(angle);
			float y = radius * sin(angle);
			float z = 0.0;

			vertices[i] = x;
			vertices[i + 1] = y;
			vertices[i + 2] = z;
		}
		else
		{
			float x = (radius / 2) * cos(angle);
			float y = (radius / 2) * sin(angle);
			float z = 0.0;

			vertices[i] = x;
			vertices[i + 1] = y;
			vertices[i + 2] = z;
		}

		i += 3;
		angle += slice;
	}
	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, totalSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}
