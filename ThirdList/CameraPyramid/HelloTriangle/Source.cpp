/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico - Jogos Digitais - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 05/03/2022
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

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
//int setupShader();
int setupGeometry();
int setupGeometry3D();
// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 600, HEIGHT = 600;
int viewID = 1;
// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();


	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! Brenda", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
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

	GLuint VAO = setupGeometry3D();

	glUseProgram(shader.ID);

	//Matriz de View
	
	glm::mat4 view = glm::mat4(1);
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("view", glm::value_ptr(view));

	//Matriz proje��o
	glm::mat4 projection = glm::mat4(1);
	projection = glm::perspective(45.0f,(GLfloat) WIDTH/(GLfloat)HEIGHT,0.1f,100.0f);
	//d� pra usar o shader.setMat, mais f�cil.
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	// Loop da aplica��o - "game loop"

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(20);
		glPointSize(20);

		switch (viewID) 
		{
		case 1:
			view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case 2:
			view = glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case 3:
			view = glm::lookAt(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case 4:
			view = glm::lookAt(glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		}
		shader.setMat4("view", glm::value_ptr(view));
		glm::mat4 model= glm::mat4(1);
		//model = glm::rotate(model, (float)glfwGetTime()/*glm::radians(45.f)*/, glm::vec3(0, 1, 0));
		GLint modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 18);

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
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		viewID = 1; //FRENTE
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		viewID = 2; //TR�S
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		viewID = 3; //ESQUERDA
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		viewID = 4; //DIREITA
	}

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
int setupGeometry3D()
{
	// Aqui setamos as coordenadas x, y e z do tri�ngulo e as armazenamos de forma
	// sequencial, j� visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v�rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO �nico ou em VBOs separados
	GLfloat vertices[] = {
		//Base da pir�mide: 2 tri�ngulos
		//x    y    z    r    g    b
		-0.5, -0.5, -0.5, 1.0, 1.0, 0.0,
		-0.5, -0.5,  0.5, 0.0, 1.0, 1.0,
		 0.5, -0.5, -0.5, 1.0, 0.0, 1.0,
		 -0.5, -0.5, 0.5, 1.0, 1.0, 0.0,
		  0.5, -0.5,  0.5, 0.0, 1.0, 1.0,
		  0.5, -0.5, -0.5, 1.0, 0.0, 1.0,
		  //
		  -0.5, -0.5, -0.5, 1.0, 1.0, 0.0,
		   0.0,  0.5,  0.0, 1.0, 1.0, 0.0,
		   0.5, -0.5, -0.5, 1.0, 1.0, 0.0,
		  -0.5, -0.5, -0.5, 1.0, 0.0, 1.0,
		   0.0,  0.5,  0.0, 1.0, 0.0, 1.0,
		  -0.5, -0.5,  0.5, 1.0, 0.0, 1.0,
		  -0.5, -0.5, 0.5, 1.0, 1.0, 0.0,
		   0.0,  0.5, 0.0, 1.0, 1.0, 0.0,
		   0.5, -0.5, 0.5, 1.0, 1.0, 0.0,
		   0.5, -0.5, 0.5, 0.0, 1.0, 1.0,
		   0.0,  0.5,  0.0, 0.0, 1.0, 1.0,
		   0.5, -0.5, -0.5, 0.0, 1.0, 1.0,
	};
	GLuint VBO, VAO;
	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posi��o (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);
	return VAO;
}
