/*
Práctica 7: Iluminación 1
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

// Booleano para Iluminacion
bool luz_PA = true;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

//Dado 8 caras
Texture dado8caras;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

//Nombramos modelos carro
Model carro;
Model capo_carro;
Model rueda_derA;
Model rueda_derF;
Model rueda_izqA;
Model rueda_izqF;

//Modelo lampara
Model lampara;

//Modelo corona
Model corona;


//Bandera para la luz del carro (Determina si esta encendido una u otra luz)
//Tambien se colocan variables para tener control de la distancia y el movimiento
static double Datos = 0.0;
static double Final = 0.0;
bool luz_carro = false;



Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}



void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}




void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

//Dado 8 caras 
void CrearOctaedro()
{
	unsigned int octaedro_indices[] = {
		// Las 8 caras triangulares del octaedro
		0, 1, 2,  // Cara 1 (número 8)
		3, 4, 5,  // Cara 2 (número 2)
		6, 7, 8,  // Cara 3 (número 6)
		9, 10, 11, // Cara 4 (número 4)
		12, 13, 14, // Cara 5 (número 1)
		15, 16, 17, // Cara 6 (número 7)
		18, 19, 20, // Cara 7 (número 3)
		21, 22, 23  // Cara 8 (número 5)
	};

	GLfloat octaedro_vertices[] = {
		// Cara 1 (número 8)
		0.0f,   1.0f,   0.0f,   0.25f,  0.25f,     1.0f, -1.0f, -1.0f,
		-1.0f,  0.0f,   1.0f,   0.125f, 0.5f,      1.0f, -1.0f, -1.0f,
		1.0f,   0.0f,   1.0f,   0.375f, 0.5f,      1.0f, -1.0f, -1.0f,

		// Cara 2 (número 2)
		0.0f,   1.0f,   0.0f,   0.625f, 0.25f,     -1.0f, -1.0f, -1.0f,
		1.0f,   0.0f,   1.0f,   0.5f,   0.5f,      -1.0f, -1.0f, -1.0f,
		1.0f,   0.0f,  -1.0f,   0.75f,  0.5f,      -1.0f, -1.0f, -1.0f,

		// Cara 3 (número 6)
		0.0f,   1.0f,   0.0f,   0.125f, 0.25f,     -1.0f, -1.0f, -1.0f,
		1.0f,   0.0f,  -1.0f,   0.0f,   0.5f,      -1.0f, -1.0f, -1.0f,
		-1.0f,  0.0f,  -1.0f,   0.25f,  0.5f,      -1.0f, -1.0f, -1.0f,

		// Cara 4 (número 4)
		0.0f,   1.0f,   0.0f,   0.5f,   0.25f,     1.0f, -1.0f, -1.0f,
		-1.0f,  0.0f,  -1.0f,   0.375f, 0.5f,      1.0f, -1.0f, -1.0f,
		-1.0f,  0.0f,   1.0f,   0.625f, 0.5f,      1.0f, -1.0f, -1.0f,

		// Cara 5 (número 1)
		0.0f,  -1.0f,   0.0f,   0.25f,  0.75f,     1.0f, 1.0f, 1.0f,
		-1.0f,  0.0f,   1.0f,   0.125f, 1.0f,      1.0f, 1.0f, 1.0f,
		1.0f,   0.0f,   1.0f,   0.375f, 1.0f,      1.0f, 1.0f, 1.0f,

		// Cara 6 (número 7)
		0.0f,  -1.0f,   0.0f,   0.625f, 0.75f,     -1.0f, 1.0f, 1.0f,
		1.0f,   0.0f,   1.0f,   0.5f,   1.0f,      -1.0f, 1.0f, 1.0f,
		1.0f,   0.0f,  -1.0f,   0.75f,  1.0f,      -1.0f, 1.0f, 1.0f,

		// Cara 7 (número 3)
		0.0f,  -1.0f,   0.0f,   0.75f,  0.75f,     1.0f, 1.0f, 1.0f,
		1.0f,   0.0f,  -1.0f,   0.625f, 1.0f,      1.0f, 1.0f, 1.0f,
		-1.0f,  0.0f,  -1.0f,   0.875f, 1.0f,      1.0f, 1.0f, 1.0f,

		// Cara 8 (número 5)
		0.0f,  -1.0f,   0.0f,   0.5f,   0.75f,     1.0f, -1.0f, 1.0f,
		-1.0f,  0.0f,  -1.0f,   0.375f, 1.0f,      1.0f, -1.0f, 1.0f,
		-1.0f,  0.0f,   1.0f,   0.625f, 1.0f,      1.0f, -1.0f, 1.0f
	};




	Mesh* octaedro = new Mesh();
	octaedro->CreateMesh(octaedro_vertices, octaedro_indices, 192, 24); // 24 vértices * 8 datos = 192, 8 caras * 3 índices = 24
	meshList.push_back(octaedro);


}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearOctaedro();


	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	//Dado
	dado8caras = Texture("Textures/dado8caras.png");
	dado8caras.LoadTextureA();



	/*Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");*/

	//Helicoptero
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	//Modelos carro 
	carro = Model();
	carro.LoadModel("Models/carro.obj");

	capo_carro = Model();
	capo_carro.LoadModel("Models/capo_carro.obj");

	rueda_izqF = Model();
	rueda_izqF.LoadModel("Models/rueda_izqF.obj");

	rueda_izqA = Model();
	rueda_izqA.LoadModel("Models/rueda_izqA.obj");

	rueda_derA = Model();
	rueda_derA.LoadModel("Models/rueda_derA.obj");

	rueda_derF = Model();
	rueda_derF.LoadModel("Models/rueda_derF.obj");

	//Modelos corona

	corona = Model();
	corona.LoadModel("Models/corona.obj");


	//Modelos lampara
	lampara = Model();
	lampara.LoadModel("Models/lampara.obj");



	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;





	//Declaracion de luz puntual Para la corona
	pointLights[0] = PointLight(0.7f, 0.0f, 0.0f,
		0.6f, 0.6f,
		-0.2f, 1.5f, 16.5f,
		0.0f, 0.1f, 0.06f);
	pointLightCount++;

	//Declaración de luz puntual para la lampara
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		0.1f, 0.1f,
		-15.0f, 3.0f, 13.5f,
		0.0f, 0.1f, 0.06f);
	pointLightCount++;






	// Declaración de segunda luz puntual (para lampara)


	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f, // Ilumina un punto
		0.0f, 0.0f, 0.0f, // Origen
		0.0f, -1.0f, 0.0f, // Apunta
		1.0f, 0.0f, 0.0f, // Con, Lin, Exp
		10.0f); // Abertura de luz
	spotLightCount++;

	//luz fija
	//Luz delantera del carro 
	spotLights[1] = SpotLight(
		0.0f, 0.0f, 1.0f, // azul
		1.0f, 2.0f, //Ilumina a su alrededor
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 
	//Cofre
	spotLights[2] = SpotLight(
		1.0f, 0.0f, 0.0f,       // Color rojo
		1.0f, 2.0f,             // Intensidad
		1.58f, 1.0f, 0.0f,      // Posición inicial (misma que el cofre)
		0.0f, -1.0f, 0.0f,      // Dirección inicial (hacia abajo)
		1.0f, 0.1f, 0.01f,      // Parámetros de atenuación
		30.0f);                 // Ángulo de apertura
	spotLightCount++;

	// luz fija
	//Luz Trasera del carro
	spotLights[3] = SpotLight(
		0.0f, 1.0f, 0.0f, // Amarilla
		1.0f, 1.0f, // Ilumina a su alrededor
		1.58f, 1.0f, 0.0f, // Origen
		1.0f, 0.0f, 0.0f, // Apunta 
		0.0f, 0.2f, 0.1f, // Con, Lin, Exp (ajusta estos valores para la atenuación)
		40.0f); // Abertura de luz 
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);






		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		
		//
		////Dado
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(1.0f, 10.0f, 4.0f)); // Posición en el mundo
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));        // Escala
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//dado8caras.UseTexture();
		//meshList[4]->RenderMesh();


		//Cuerpo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.95f, 3.0f)); // Bajamos el carro
		model = glm::translate(model, glm::vec3(mainWindow.getarticulacion7(), 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(mainWindow.getarticulacion8(), 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f)); 
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		
		glm::vec3 Faro = glm::vec3(0.0f + mainWindow.getarticulacion7()+ mainWindow.getarticulacion8(), 1.0f, 3.0f);
		spotLights[1].SetFlash(Faro, glm::vec3(-1.0f, 0.0f, 0.0f));

		glm::vec3 Faro_trasero = glm::vec3(12.0f + mainWindow.getarticulacion7() + mainWindow.getarticulacion8(), 1.5f, 4.0f);
		spotLights[3].SetFlash(Faro_trasero, glm::vec3(1.0f, 0.0f, 0.0f));


		
		Final = Datos;

		Datos = mainWindow.getarticulacion7() + mainWindow.getarticulacion8();

		if (Datos > Final) {
			Final = -5;  // Si Decision es mayor que des, se resta des de Decision
		}
		else if (Datos < Final) {
			Final = 5;  // Si Decision es menor que des, se resta Decision de des
		}


		// Actualizar el estado de la luz basado en la señal de articulacion3
		if (Final == -5) {
			luz_carro = true;
		}
		else if (Final == 5) {
			luz_carro = false;
		}

		// Aplicar el estado actual de la luz
		if (luz_carro) {

			spotLights[1].SetFlash(Faro, glm::vec3(0.0f, 0.0f, 0.0f));
			spotLights[3].SetFlash(Faro_trasero, glm::vec3(1.0f, 0.0f, 0.0f));


		}
		else {

			spotLights[1].SetFlash(Faro, glm::vec3(-1.0f, 0.0f, 0.0f));
			spotLights[3].SetFlash(Faro_trasero, glm::vec3(0.0f, 0.0f, 0.0f));


		}


		



		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carro.RenderModel();


		//Capo
		model = modelaux;  // Asegúrate de que modelaux ya esté definido
		model = glm::translate(model, glm::vec3(1.58f, 1.0f, 0.0f));
		float anguloRotacionCofre = mainWindow.getarticulacion2();  // Usa tu variable de articulación
		model = glm::rotate(model, glm::radians(anguloRotacionCofre), glm::vec3(0.0f, 0.0f, 1.0f));







		// Establece la posición base del capó
		glm::vec3 capoPosition = glm::vec3(-5.4f, 2.5f, 2.0f);

		// Obtén los valores de las articulaciones 7 y 8 (estos valores deberían obtenerse de métodos en `mainWindow`)
		float desplazamientoX = mainWindow.getarticulacion7() + mainWindow.getarticulacion8();  // Suma los desplazamientos de las articulaciones en el eje X

		// Actualiza la posición del capó sumando el desplazamiento calculado al valor de X
		capoPosition.x += desplazamientoX;  // La posición del capó se ajusta en el eje X según las articulaciones

		// Dirección inicial de la luz (suponiendo que originalmente apunta hacia el eje X negativo)
		glm::vec3 direccionLuzOriginal = glm::vec3(-1.0f, 0.0f, 0.0f);

		// Aplica una rotación a la dirección de la luz basada en la rotación del modelo (en el eje Z)
		glm::mat4 rotacionLuz = glm::rotate(glm::mat4(1.0f), glm::radians(-mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 direccionLuzRotada = glm::vec3(rotacionLuz * glm::vec4(direccionLuzOriginal, 0.0f));

		// Aplica la nueva posición y dirección de la luz al tercer spot light
		spotLights[2].SetFlash(capoPosition, direccionLuzRotada);  // La luz se desplaza en el eje X y rota con el capó










		// Continúa con el renderizado del cofre
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));  // Asegúrate de que uniformModel ya esté definido
		capo_carro.RenderModel();  // Asegúrate de que capo_carro ya esté definido




		//Rueda izquierda frontal
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.0f, 0.3f, -0.85f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rueda_izqF.RenderModel();

		//Rueda derecha frontal
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.0f, 0.3f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rueda_derF.RenderModel();


		// Rueda izquierda trasera 
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.05f, 0.3f, -0.9f)); // Ajuste leve en X y Z
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rueda_izqA.RenderModel();


		// Rueda derecha trasera 
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.0f, 0.3f, 0.85f)); // X simétrico, Z ajustado
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rueda_derA.RenderModel();

		//Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.3f, 15.0f)); // X simétrico, Z ajustado
		modelaux = model;
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//Llamamos la luz


		//Llamamos la luz
		// Verificar el valor de articulacion13 para encender o apagar la luz
		if (mainWindow.getarticulacion13() == 1) {
			luz_PA = true;
		}
		else if (mainWindow.getarticulacion13() == 0) {
			luz_PA = false;
		}

		// Activar o desactivar la luz según su estado actual
		if (luz_PA) {
			shaderList[0].SetPointLights(&pointLights[1], 1);
		}
		else {

			// La luz se apagada
			shaderList[0].SetPointLights(&pointLights[1], 0);
		}

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();





		//Corona
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.0f, 0.3f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//Llamamos la luz
		// Verificar el valor de articulacion11 para encender o apagar la luz
		if (mainWindow.getarticulacion11() == 1) {
			luz_PA = true;
		}
		else if (mainWindow.getarticulacion11() == 0) {
			luz_PA = false;
		}

		// Activar o desactivar la luz según su estado actual
		if (luz_PA) {
			shaderList[0].SetPointLights(&pointLights[0], 1);
		}
		else {

			// La luz se apagada
			shaderList[0].SetPointLights(&pointLights[0], 0);
		}



		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		corona.RenderModel();





		//// Helicoptero
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(10.0f, 16.0f, 13.0f));
		//model = glm::translate(model, glm::vec3(mainWindow.getarticulacion9(), 0.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(mainWindow.getarticulacion10(), 0.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		////Llamamos la luz 
		//glm::vec3 luz_helicoptero = glm::vec3(20.0f + mainWindow.getarticulacion9()+mainWindow.getarticulacion10(), 7.0f, 13.0f);
		//spotLights[3].SetFlash(luz_helicoptero, glm::vec3(0.0f, -1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Blackhawk_M.RenderModel();
		

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}