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


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupHouse();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 450\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
//...pode ter mais linhas de código aqui!
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

//Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 450\n"
"uniform vec4 inputColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = inputColor;\n"
"}\n\0";

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

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupHouse();

	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
	assert(colorLoc > -1);
	
	glUseProgram(shaderID);
	

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);
		
		//Quadrado e Telhado: contorno
		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 0.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_LOOP, 0, 7);
		
		//Cor do Telhado
		glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 4, 3);

		//Chão
		glUniform4f(colorLoc, 1.0f, 0.5f, 0.0f, 0.0f);
		glDrawArrays(GL_LINES, 7, 2);

		//Contorno Porta e Cor dela
		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 0.0f);
		glDrawArrays(GL_LINES, 9, 2);
		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 0.0f);
		glDrawArrays(GL_LINES, 10, 2);
		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 0.0f);
		glDrawArrays(GL_LINES, 11, 2);
		glUniform4f(colorLoc, 0.5f, 0.35f, 0.05f, 0.0f);
		glDrawArrays(GL_TRIANGLE_FAN, 9, 4);
		
		//Contorno da Janela e Cor
		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 0.0f);
		glDrawArrays(GL_LINE_LOOP, 13, 4);
		glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLE_FAN, 13, 4);

		//Cruz da Janela
		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 0.0f);
		glDrawArrays(GL_LINES, 19, 2);
		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 0.0f);
		glDrawArrays(GL_LINES, 17, 2);

		glBindVertexArray(0);

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

int setupShader()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int setupHouse()
{

	GLfloat housebase[] = {
		 0.0, 0.5, 0.0, //A
		 0.5, 0.5, 0.0, //B
		 0.5, 0.0, 0.0, //C
		 0.0, 0.0, 0.0, //D
		 //quadrado grande

		 0.0, 0.5, 0.0,
		 0.5, 0.5, 0.0,
		 0.2, 0.8, 0.0,
		 //telhado

		 -0.1, 0.0, 0.0,
		  0.6, 0.0, 0.0,
		 //chão
		  
		0.2, 0.0, 0.0,//H
		0.2, 0.2, 0.0,//I
		0.3, 0.2, 0.0,//J
		0.3, 0.0, 0.0,//K
		//porta

		0.1, 0.3, 0.0,//M
		0.1, 0.4, 0.0,//L
		0.2, 0.4, 0.0,//O
		0.2, 0.3, 0.0,//N
		//janela

		0.15, 0.3, 0.0,//P
		0.15, 0.4, 0.0,//Q
		0.1, 0.35, 0.0,//S
		0.2, 0.35, 0.0,//R
		//cruz janela
	};
	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(housebase), housebase, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); 

	return VAO;
}

