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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>
#include "Shader.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
//int setupShader();
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 600, HEIGHT = 600;


// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();


	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! Brenda", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	Shader shader("../Shaders/Ortho.vs", "../Shaders/Ortho.fs");

	GLuint VAO = setupGeometry();

	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);

	glUseProgram(shader.ID);

	glm::mat4 projection = glm::mat4(1);
	projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(20);
		glPointSize(20);

		glm::mat4 model= glm::mat4(1);

		glViewport(0, 0, width / 2, height / 2);
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 0, 1));
		GLint modelLocR1 = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLocR1, 1, FALSE, glm::value_ptr(model));
		
		//triangulo 1
		glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glViewport(width / 2, 0, width / 2, height / 2);
		model = glm::rotate(model, glm::radians(64.0f), glm::vec3(0, 0, 1));
		GLint modelLocR2 = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLocR2, 1, FALSE, glm::value_ptr(model));
		
		//triangulo 2
		glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glViewport(0, height / 2, width / 2, height / 2);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
		GLint modelLocR3 = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLocR3, 1, FALSE, glm::value_ptr(model));

		//triangulo 3
		glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Troca os buffers da tela
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

int setupGeometry()
{
	GLfloat vertices[] = {
		//x   y     z
		-0.5, 0.0, 0.0,
		 0.0, 0.0, 0.0,
		-0.25, 0.5, 0.0,
		//outro triangulo vai aqui
	};

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

