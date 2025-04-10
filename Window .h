#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }


	//Articulaciones carro

	//art carro base
	GLfloat getarticulacion1() { return articulacion1; }
	//art capo
	GLfloat getarticulacion2() { return articulacion2; }
	//art rif
	GLfloat getarticulacion3() { return articulacion3; }
	//art rdf
	GLfloat getarticulacion4() { return articulacion4; }
	//pata ria
	GLfloat getarticulacion5() { return articulacion5; }
	//pata rda
	GLfloat getarticulacion6() { return articulacion6; }

	//translate
	GLfloat getarticulacion7() { return articulacion7; }


	//arts extra

	GLfloat getarticulacion8() { return articulacion8; }

	//Helicoptero 
	GLfloat getarticulacion9() { return articulacion9; }
	GLfloat getarticulacion10() { return articulacion10; }


	GLfloat getarticulacion11() { return articulacion11; }
	GLfloat getarticulacion12() { return articulacion12; }
	GLfloat getarticulacion13() { return articulacion13; }




	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLfloat rotax, rotay, rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6, articulacion7, articulacion8, articulacion9, articulacion10, articulacion11, articulacion12, articulacion13;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};