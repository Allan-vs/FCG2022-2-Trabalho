/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 01/08/2022
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

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Shader.h"

//using namespace glm;

#include "Lista.h"
#include "No.h"

// Protótipo da função de callback de teclado e de mouse
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Protótipos das funções
int setupGeometry();
int setupGeometry3D();

void updateCameraPos(GLFWwindow* window);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Variáveis globais para controle de câmera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); 
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float lastX = WIDTH / 2.0, lastY = HEIGHT / 2.0; //para calcular o quanto que o mouse deslocou
float yaw = -90.0, pitch = 0.0; //rotação em x e y


Lista grid; //criação da lista das grids, onde vão ter os índices

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif


	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Voxel", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	//Desabilitando o cursor do mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	


	// Compilando e buildando o programa de shader
	Shader shader("../shaders/hello_triangle.vs", "../shaders/hello_triangle.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry3D();//setupGeometry();
	
	//Habilitar a transparência
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	
	glUseProgram(shader.ID);

	//Matriz de projeção PERSPECTIVA
	glm::mat4 projection = glm::mat4(1); //matriz identidade
	//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	shader.setMat4("projection", glm::value_ptr(projection));
	
	//Matriz de view
	glm::mat4 view = glm::mat4(1);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),// Posição (ponto) 
		   glm::vec3(0.0f, 0.0f, 0.0f),// Target (ponto, não vetor)  dir = target - pos                
		   glm::vec3(0.0f, 1.0f, 0.0f)); // Up (vetor)
	shader.setMat4("view", glm::value_ptr(view));

	//Habilita teste de profundidade
	glEnable(GL_DEPTH_TEST);


	//For para criação da lista de indices da grid
	for (int i = 0; i < 1000; i++) { 
		grid.inserirLista(-1); //-1 para todos os voxels não preenchidos
	}


	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();
		updateCameraPos(window);

		//Atualiza a matriz de view, afinal a camera pode mudar de posição e orientação
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader.setMat4("view", glm::value_ptr(view));

		// Limpa o buffer de cor
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(1);
		glPointSize(20);


		//Viewport 
		glViewport(0, 0, width, height);

		//Definição da matriz de modelo (transf. na geometria)
		glm::mat4 model = glm::mat4(1); //matriz identidade
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, cos((float)glfwGetTime())));
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1, 0, 0));

		shader.setMat4("model", glm::value_ptr(model));


		glBindVertexArray(VAO);

		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		//shader.setVec4("inputColor", 1.0, 0.0, 0.0, 1.0);
		///glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); //enviando cor para variável uniform inputColor		
		//glDrawArrays(GL_TRIANGLES, 0, 108000);
		
		
		shader.setVec4("inputColor", 0.0, 0.0, 0.0, 0.5);
		for (int i = 0; i < grid.quantidadeElementos(); i++) {
			glDrawArrays(GL_LINE_LOOP, i*36, 36);
		}
		
		
		//glDrawArrays(GL_LINE_LOOP, 0, 36);
		//glDrawArrays(GL_TRIANGLES, 0, 108);
		
		//GRID
		for (int i = 0; i < grid.quantidadeElementos(); i++) { //percorre a lista de indexes (grid)
			if (grid.buscarLista(i) > -1) { //verifica se é para desenhar o voxel
				switch (grid.buscarLista(i)) { //seleciona a cor
					case 0: shader.setVec4("inputColor", 1.0, 1.0, 1.0, 1.0); break; //BRANCO
					case 1: shader.setVec4("inputColor", 1.0, 0.0, 0.0, 1.0); break; //VERMELHO
					case 2: shader.setVec4("inputColor", 0.0, 1.0, 0.0, 1.0); break; //VERDE
					case 3: shader.setVec4("inputColor", 0.0, 0.0, 1.0, 1.0); break; //AZUL
					case 4: shader.setVec4("inputColor", 0.0, 1.0, 1.0, 1.0); break; //CIANO
					case 5: shader.setVec4("inputColor", 1.0, 0.0, 1.0, 1.0); break; //MAGENTA
					case 6: shader.setVec4("inputColor", 1.0, 1.0, 0.0, 1.0); break; //AMARELO
				}
				glDrawArrays(GL_TRIANGLES, i*36, 36);
			}
		}

		//CURSOR
		shader.setVec4("inputColor", 1.0, 1.0, 1.0, 0.3);
		glDrawArrays(GL_TRIANGLES, grid.obterPosCursor()*36, 36);

		
		//grid.alterarIndex(0, 0);
		//grid.alterarIndex(1, 0);
		//grid.alterarIndex(9, 0);
		
		glBindVertexArray(0); //"unbind do VAO" 

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
	//Fechar Janela
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	//Movimentação cursor
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		grid.moverCursor(-1);
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		grid.moverCursor(1);
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		grid.moverCursor(100);
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		grid.moverCursor(-100);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		grid.moverCursor(10);
	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
		grid.moverCursor(-10);
	//Colocar um Voxel
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
		grid.alterarIndex(grid.obterPosCursor(), 0);
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		grid.alterarIndex(grid.obterPosCursor(), 1);
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		grid.alterarIndex(grid.obterPosCursor(), 2);
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		grid.alterarIndex(grid.obterPosCursor(), 3);
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		grid.alterarIndex(grid.obterPosCursor(), 4);
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
		grid.alterarIndex(grid.obterPosCursor(), 5);
	if (key == GLFW_KEY_6 && action == GLFW_PRESS)
		grid.alterarIndex(grid.obterPosCursor(), 6);
	//Apagar um Voxel
	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
		grid.alterarIndex(grid.obterPosCursor(), -1);
	//Apagar tudo
	if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
		grid.esvaziaLista(); //Apaga a lista
		for (int i = 0; i < 1000; i++) { //Recria ela com os Voxels vazios
			grid.inserirLista(-1);
		}
	}
	//Salvar lista de indexes
	if (key == GLFW_KEY_F7 && action == GLFW_PRESS)
		grid.salvaLista();
	//Carrega lista de indexes previamente salva
	if (key == GLFW_KEY_F8 && action == GLFW_PRESS)
		grid.carregaLista();
}

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat vertices[] = { //Ex3)
		-0.5, -0.5, 0.0, //v0
		 0.5, -0.5, 0.0, //v1
		 0.0, 0.5, 0.0,  //v2
		 //outro triangulo vai aqui
	};

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0); 

	return VAO;
}

int setupGeometry3D()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	
	//vertices totais da grid
	GLfloat vertices[108000];
	//Vertices de um cubo
	GLfloat vertices_aux[]{
		//x    y    z 
		//lado 1
		-0.1, -0.1, -0.1,
		-0.1, -0.1, 0.1,
		0.1, -0.1, -0.1,
		-0.1, -0.1, 0.1,
		0.1, -0.1, 0.1,
		0.1, -0.1, -0.1,
		//lado 2
		-0.1, -0.1, -0.1,
		0.1, -0.1, -0.1,
		-0.1, 0.1, -0.1,
		-0.1, 0.1, -0.1,
		0.1, -0.1, -0.1,
		0.1, 0.1, -0.1,
		//lado 3
		-0.1, -0.1, 0.1,
		-0.1, -0.1, -0.1,
		-0.1, 0.1, 0.1,
		-0.1, 0.1, 0.1,
		-0.1, 0.1, -0.1,
		-0.1, -0.1, -0.1,
		//lado 4
		0.1, 0.1, 0.1,
		0.1, 0.1, -0.1,
		-0.1, 0.1, 0.1,
		0.1, 0.1, -0.1,
		-0.1, 0.1, -0.1,
		-0.1, 0.1, 0.1,
		//lado 5
		0.1, 0.1, 0.1,
		-0.1, 0.1, 0.1,
		0.1, -0.1, 0.1,
		0.1, -0.1, 0.1,
		-0.1, 0.1, 0.1,
		-0.1, -0.1, 0.1,
		//lado 6
		0.1, 0.1, -0.1,
		0.1, 0.1, 0.1,
		0.1, -0.1, -0.1,
		0.1, -0.1, -0.1,
		0.1, -0.1, 0.1,
		0.1, 0.1, 0.1,
	};
	
	//vertices de um cubo, para auxiliar a criação da grid
	

	int ia = 0, iv = 0; //ia = index auxiliar para saber qual coordenada tem que add ---- iv = index dos vertices
	float iax = 0, iay = 0, iaz = 0; //index auxiliar de cada coordenada, para "mover" o cubo
	
	for (int i = 0; i < 10; i++) { //for do z
		for (int i = 0; i < 10; i++) { //for do y
			for (int i = 0; i < 10; i++) { //for do x
				for (int iva = 0; iva < 108; iva++) { //percorre as coordenadas
					if (ia == 0) { 
						vertices[iv] = vertices_aux[iva] + iax; //adição das coordenadas x
					}
					else if (ia == 1) {
						vertices[iv] = vertices_aux[iva] + iay; //adição das coordenadas y
					}
					else if (ia == 2) {
						vertices[iv] = vertices_aux[iva] + iaz; //adição das coordenadas Z
					}
					ia++;

					if (ia == 3) {
						ia = 0;
					}
					iv++;
				}
				iax = iax + 0.2; //soma x
			} 
			iax = 0; //reseta x
			iay = iay + 0.2; //soma y
		}
		iay = 0; //reseta y
		iaz = iaz + 0.2; //soma z
	}
	
	
	

	GLuint VBO, VAO;
	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); //alterei para 3
	glEnableVertexAttribArray(0);
	//Atributo cor (r, g, b)
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);
	return VAO;
}

void updateCameraPos(GLFWwindow* window)
{
	float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	//Precisamos também atualizar o cameraUp!! Pra isso, usamos o Up do  
	//mundo (y), recalculamos Right e depois o Up
	glm::vec3 right = glm::normalize(glm::cross(cameraFront,
		glm::vec3(0.0, 1.0, 0.0)));
	cameraUp = glm::normalize(glm::cross(right, cameraFront));
}


