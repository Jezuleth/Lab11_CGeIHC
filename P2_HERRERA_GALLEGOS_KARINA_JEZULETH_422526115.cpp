// "Practica 2: Proyecciones, transformaciones HERRERA GALLEGOS KARIMA JEZULETH 422526115"
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Asignamos las variables que enlazan y manejan nuestros archivos de shaders.(Vertex shaders)
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
static const char* FRojo = "shaders/FRojo.frag";
static const char* VRojo = "shaders/VRojo.vert";
static const char* FVerde = "shaders/FVerde.frag";
static const char* VVerde = "shaders/VVerde.vert";
static const char* FAzul = "shaders/FAzul.frag";
static const char* VAzul = "shaders/VAzul.vert";
static const char* FCafe = "shaders/FCafe.frag";
static const char* VCafe = "shaders/VCafe.vert";
static const char* FVerdeOscuro = "shaders/FVerdeOscuro.frag";
static const char* VVerdeOscuro = "shaders/VVerdeOscuro.vert";

float angulo = 0.0f;

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,
		0.5f,-0.5f,0.0f,	
		0.0f,0.5f, -0.25f,	
		0.0f,-0.5f,-0.5f,	

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12); 
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// frontal
		0, 1, 2,
		2, 3, 0,
		// der
		1, 5, 6,
		6, 2, 1,
		// atras
		7, 6, 5,
		5, 4, 7,
		// izq
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// Parte Frontal
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// Parte Trasera
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
		//X	 Y		Z		R	G	B
	// Letra J  (144)
		// Triángulo 1 (Horizontal)
	// Triángulo 1 (Horizontal)
	-0.9f,  0.4f, 0.0f, 1.0f,1.0f,0.0f,
	-0.5f,  0.4f, 0.0f, 1.0f,1.0f,0.0f,
	-0.5f,  0.5f, 0.0f, 1.0f,1.0f,0.0f,
	// Triángulo 2 (Horizontal)
	-0.9f,  0.4f, 0.0f, 1.0f,1.0f,0.0f,
	-0.9f,  0.5f, 0.0f, 1.0f,1.0f,0.0f,
	-0.5f,  0.5f, 0.0f, 1.0f,1.0f,0.0f,
	// Triángulo 3 (Cuadrado)
	-0.9f,  0.5f, 0.0f, 1.0f,1.0f,0.0f,
	-0.8f,  0.5f, 0.0f, 1.0f,1.0f,0.0f,
	-0.8f,  0.6f, 0.0f, 1.0f,1.0f,0.0f,
	// Triángulo 4 (Cuadrado)
	-0.9f,  0.5f, 0.0f, 1.0f,1.0f,0.0f,
	-0.9f,  0.6f, 0.0f, 1.0f,1.0f,0.0f,
	-0.8f,  0.6f, 0.0f, 1.0f,1.0f,0.0f,
	// Triángulo 5 (Vertical)
	-0.6f,  0.5f, 0.0f, 1.0f,1.0f,0.0f,
	-0.5f,  0.5f, 0.0f, 1.0f,1.0f,0.0f,
	-0.5f,  0.9f, 0.0f, 1.0f,1.0f,0.0f,
	// Triángulo 6 (Vertical)
	-0.6f,  0.5f, 0.0f, 1.0f,1.0f,0.0f,
	-0.5f,  0.9f, 0.0f, 1.0f,1.0f,0.0f,
	-0.6f,  0.9f, 0.0f, 1.0f,1.0f,0.0f,
	// Triángulo 7 (Horizontal)
	-0.8f,  0.9f, 0.0f, 1.0f,1.0f,0.0f,
	-0.4f,  0.9f, 0.0f, 1.0f,1.0f,0.0f,
	-0.4f,  1.0f, 0.0f, 1.0f,1.0f,0.0f,
	// Triángulo 8 (Horizontal)
	-0.8f,  1.0f, 0.0f, 1.0f,1.0f,0.0f,
	-0.4f,  1.0f, 0.0f, 1.0f,1.0f,0.0f,
	-0.8f,  0.9f, 0.0f, 1.0f,1.0f,0.0f,
	// Letra H  (108)
	// Triángulo 1 (Vertical)
	-0.3f,  0.4f, 0.0f, 255.0f, 0.0f, 255.0f,
	-0.2f,  0.4f, 0.0f, 255.0f, 0.0f, 255.0f,
	-0.3f,  1.0f, 0.0f, 255.0f, 0.0f, 255.0f,
	// Triángulo 2 (Vertical)
	-0.3f,  1.0f, 0.0f, 255.0f, 0.0f, 255.0f,
	-0.2f,  1.0f, 0.0f, 255.0f, 0.0f, 255.0f,
	-0.2f,  0.4f, 0.0f, 255.0f, 0.0f, 255.0f,
	// Triángulo 3 (Horizontal)
	-0.2f,  0.7f, 0.0f, 255.0f, 0.0f, 255.0f,
	0.0f,  0.7f, 0.0f, 255.0f, 0.0f, 255.0f,
	0.0f,  0.8f, 0.0f, 255.0f, 0.0f, 255.0f,
	// Triángulo 4 (Horizontal)
	-0.2f,  0.7f, 0.0f, 255.0f, 0.0f, 255.0f,
	0.0f,  0.8f, 0.0f, 255.0f, 0.0f, 255.0f,
	-0.2f,  0.8f, 0.0f, 255.0f, 0.0f, 255.0f,
	// Triángulo 5 (Vertical)
	0.0f,  0.4f, 0.0f, 255.0f, 0.0f, 255.0f,
	0.1f,  0.4f, 0.0f, 255.0f, 0.0f, 255.0f,
	0.1f,  1.0f, 0.0f, 255.0f, 0.0f, 255.0f,
	// Triángulo 6 (Vertical)
	0.0f,  0.4f, 0.0f, 255.0f, 0.0f, 255.0f,
	0.1f,  1.0f, 0.0f, 255.0f, 0.0f, 255.0f,
	0.0f,  1.0f, 0.0f, 255.0f, 0.0f, 255.0f,
	// Letra G (198)
	// Triángulo 1 (Vertical)
	0.2f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.3f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.3f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
	// Triángulo 2 (Vertical)
	0.2f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.3f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.2f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	// Triángulo 3 (Horizontal)
	0.2f,  0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.6f,  0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.6f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	// Triángulo 4 (Horizontal)
	0.2f,  0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.6f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.2f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	// Triángulo 5 (Horizontal)
	0.2f,  0.7f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.2f,  0.6f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.6f,  0.7f, 0.0f, 1.0f, 0.0f, 0.0f,
	// Triángulo 6 (Horizontal)
	0.6f,  0.7f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.6f,  0.6f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.2f,  0.6f, 0.0f, 1.0f, 0.0f, 0.0f,
	// Triángulo 7 (Vertical)
	0.6f,  0.7f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f,  0.7f, 0.0f, 1.0f, 0.0f, 0.0f,
	// Triángulo 8 (Vertical)
	0.6f,  0.7f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f,  0.7f, 0.0f, 1.0f, 0.0f, 0.0f,
	// Triángulo 9 (Vertical)
	0.6f,  0.7f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.6f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
	// Triángulo 10 (Horizontal)
	0.2f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.6f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.2f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
	// Triángulo 11 (Horizontal)
	0.6f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.6f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.2f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
	};
	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 450); //Sumando el total de los vertices (144+108+198)=450
	meshColorList.push_back(letras);

	GLfloat vertices_figuras[] = {
		//X			Y			Z			R		G		B
		0.0f,	0.0f,		0.0f,			0.0f,	0.0f,	1.0f, // Triangulo azul
		0.0f,	0.0f,		0.0f,			0.0f,	0.0f,	1.0f,
		0.0f,	0.0f,		0.0f,			0.0f,	0.0f,	1.0f,
	};

	MeshColor* figuras = new MeshColor();
	figuras->CreateMeshColor(vertices_figuras, 18);
	meshColorList.push_back(figuras);

	GLfloat vertices_cuadrados[] = {
		//X			Y			Z			R		G		B
		-1.1f,	0.8f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.1f,	0.8f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.1f,	-0.8f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadrados = new MeshColor();
	cuadrados->CreateMeshColor(vertices_cuadrados, 36);
	meshColorList.push_back(cuadrados);

}


void CreateShaders()
{

	Shader* shader1 = new Shader(); //Indice de objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1); //shaderList[0]	

	Shader* shader2 = new Shader();//1 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	Shader* shader3 = new Shader(); //2
	shader3->CreateFromFiles(VRojo, FRojo);
	shaderList.push_back(*shader3); 

	Shader* shader4 = new Shader();//3
	shader4->CreateFromFiles(VVerde, FVerde);
	shaderList.push_back(*shader4); 

	Shader* shader5 = new Shader();//4
	shader5->CreateFromFiles(VAzul, FAzul);
	shaderList.push_back(*shader5); 

	Shader* shader6 = new Shader();//5
	shader6->CreateFromFiles(VCafe, FCafe);
	shaderList.push_back(*shader6); 

	Shader* shader7 = new Shader(); //6
	shader7->CreateFromFiles(VVerdeOscuro, FVerdeOscuro);
	shaderList.push_back(*shader7); 
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 
	CrearCubo();//índice 1 
	CrearLetrasyFiguras(); //usa MeshColor índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad


		//Usamos el shader en shaderList[1] para renderizar las letras.
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Inicializamos una matriz 4x4 para almacenar las transformaciones geométricas del modelo.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		//Usamos el shader en shaderList[0] para renderizar piramides y cubos.
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;

		//Creamos una matriz 4x4 que funcionará como modelo para registrar las transformaciones geométricas.

		//Letras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		//Cubo (casa)
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, -2.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Piramide (techo)
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.25f, -1.8f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Piramide (arbol)
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f, -0.3f, -1.8f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Cubo (arbol)
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f, -0.6f, -1.85f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Piramide (segundo arbol)
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.8f, -0.3f, -1.8f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Cubo (segundo arbol)
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.8f, -0.6f, -1.85f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Cubo (ventana)
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.26f, -0.1f, -1.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.17f, 0.17f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Cubo (Segunda ventana)
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.26f, -0.1f, -1.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.17f, 0.17f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Cubo (puerta)
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, -1.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.17f, 0.19f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/