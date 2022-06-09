/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 11/04/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

#include "Timer.h"
using namespace std;

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

const float Pi = 3.1419;
float dx, dy;
// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções - TÁ NA HORA DE REFATORAR ISSO, NÃO ACHAM? 
int setupSprite(float nFrames, float nAnimations);
int loadTextureMoving(string path);
Timer timer; //Erro aqui?
// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

//using namespace glm;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello Textures!", nullptr, nullptr);
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

	// Compilando e buildando o programa de shader
	Shader shader("../shaders/sprite.vs", "../shaders/sprite.fs");

	//Carregando uma textura e armazenando o identificador na memória
	GLuint texID = loadTextureMoving("../textures/Sans.fw.png");
	// Gerando uma geometria de quadrilátero com coordenadas de textura
	
	int nFrames = 4;
	int nAnimations = 2;
	GLuint VAO = setupSprite(nFrames, nAnimations);
	// Ativando o shader
	glUseProgram(shader.ID);

	// Associando com o shader o buffer de textura que conectaremos
	// antes de desenhar com o bindTexture
	glUniform1i(glGetUniformLocation(shader.ID, "tex_buffer0"), 0);

	//Criando a matriz de projeção usando a GLM
	glm::mat4 projection = glm::mat4(1); //matriz identidade
	projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);

	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	//Habilitando a profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	//Habilitando a transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int iFrame = 0;
	int iAnim = 0;
	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		timer.start();
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		//Criando a matriz de modelo usando a GLM
		glm::mat4 model = glm::mat4(1); //matriz identidade
		model = glm::translate(model, glm::vec3(400.0, 300.0, 0));
		//model = glm::rotate(model, (float)glfwGetTime() /*glm::radians(90.0f)*/, glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(800.0, 500.0, 1.0));
		shader.setMat4("model", glm::value_ptr(model));

		float offsetx = dx * iFrame;
		float offsety = dy * iAnim;
		shader.setVec2("offsets", offsetx, offsety);
		iFrame =(iFrame + 1) % nFrames;
		//Ativando o primeiro buffer de textura (0) e conectando ao identificador gerado
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);


		glBindTexture(GL_TEXTURE_2D, texID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		// Observe que como  usamos EBO, agora usamos a glDrawElements!



		glBindVertexArray(0); //unbind - desconecta
		glBindTexture(GL_TEXTURE_2D, 0); //unbind da textura
		
		timer.finish();
		double waitingTime = timer.calcWaitingTime(30, timer.getElapsedTimeMs()); 
		if (waitingTime) {
			std::this_thread::sleep_for(std::chrono::milliseconds((int)waitingTime));
		}
		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


int setupSprite(float nFrames, float nAnimations)
{
	GLuint VAO;
	GLuint VBO, EBO;
	 dx = 1.0 / nFrames;
	 dy = 1.0 / nAnimations;
	float vertices[] = {
		// posicoes          // cores          // coordenadas de textura
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   dx, dy, // superior direito
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   dx, 0.0f, // inferior direito
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // inferior esquerdo
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, dy,  // superior esquerdo

	};
	unsigned int indices[] = {
	0, 1, 3, // primeiro triangulo
	1, 2, 3  // segundo triangulo
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Primeiro atributo - Layout 0 - posição - 3 valores - x, y, z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Segundo atributo - Layout 1 - cor - 3 valores - r, g, b
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Terceiro atributo - Layout 2 - coordenadas de textura - 2 valores - s, t (ou u, v)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); //desvincula

	return VAO;
}

int loadTextureMoving(string path)
{
	GLuint texID;
	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Ajusta os parâmetros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Carregamento da imagem
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}


