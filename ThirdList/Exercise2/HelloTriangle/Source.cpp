#include <iostream>
#include <string>
#include <assert.h>
#include <Windows.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shader Class
#include "Shader.h"

// Biblioteca matematica C++
#include <cmath>

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int setupGeometry();
void randomColor(GLint cl);

const GLuint WIDTH = 600, HEIGHT = 600;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	srand(GetTickCount64());//semente random  (0 a 1)

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! - Anakin", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height); //viewport original

	// Compilando e buildando o programa de shader
	Shader shader("../shaders/Ortho.vs", "../shaders/Ortho.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();

	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);

	glUseProgram(shader.ID);

	glm::mat4 projection = glm::mat4(1); //matriz identidade
	projection = glm::ortho(-6.0, 6.0, -6.0, 6.0, -1.0, 1.0);

	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	float posx = -5.0, posy = 5.0;

		// Limpa o buffer de cor
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(10);
	glPointSize(15);

	//glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 1.0f);
	glBindVertexArray(VAO);

	for (int y = 0; y < 6; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			randomColor(colorLoc);

			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(posx, posy, 0.0));
			GLint modelLoc = glGetUniformLocation(shader.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			posx += 2.0;//contagem da linha
		}
		posy += -2.0;//contagem da coluna
		posx = -5.0; //reset da linha
	}
	glfwSwapBuffers(window);
	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
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
		-1.0, -1.0, 0.0,
		-1.0,  1.0, 0.0,
		 1.0, -1.0, 0.0,
		 1.0,  1.0, 0.0
	};

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

void randomColor(GLint cl)
{
	float r, g, b;
	r = (float)rand() / (float)RAND_MAX;
	g = (float)rand() / (float)RAND_MAX;
	b = (float)rand() / (float)RAND_MAX;

	glUniform4f(cl, r, g, b, 1.0f);
}