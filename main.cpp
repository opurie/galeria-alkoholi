#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shaderprogram.h"
#include "bottle.h"
#include "lodepng.h"
#include <stdlib.h>
#include <stdio.h>

float aspectRatio = 1;
float speed_x = 0;//[radiany/s]
float speed_y = 0;//[radiany/s]
float last_x = 400, last_y = 400;
double yaw, pitch;
bool firstMouse = true;
glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
#define PI 3.14

ShaderProgram* sp;
bottle* b;

GLuint bottle_tex[5], leg, table;

//Procedura obs³ugi b³êdów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//wczytywanie tekstur
GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	std::vector<unsigned char> image;   
	unsigned width, height;  

	unsigned error = lodepng::decode(image, width, height, filename);

	glGenTextures(1, &tex); 
	glBindTexture(GL_TEXTURE_2D, tex); 
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		last_x = xpos;
		last_y = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - last_x;
	float yoffset = last_y - ypos;
	last_x = xpos;
	last_y = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camFront = glm::normalize(direction);
}

void key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) {
			speed_y = -PI;
		}
		if (key == GLFW_KEY_D) {
			speed_y = PI;
		}
		if (key == GLFW_KEY_W) {
			speed_x = -PI;
		}
		if (key == GLFW_KEY_S) {
			speed_x = PI;
		}
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_A || key == GLFW_KEY_D) {
			speed_y = 0;
		}
		if (key == GLFW_KEY_W || key == GLFW_KEY_S) {
			speed_x = 0;
		}
	}
}

void windowResizecallback(GLFWwindow* window, int w, int h) {
	if (h == 0)return;
	aspectRatio = (float)w / (float)h;
	glViewport(0, 0, w, h);
}

void init(GLFWwindow* window) {
	glClearColor(0, 0, 0 , 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowSizeCallback(window, windowResizecallback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	
	b = new bottle();
	sp = new ShaderProgram("vs.glsl", NULL,"fs.glsl");
	//sp = new ShaderProgram("V_lambert.glsl", NULL, "f_lambert.glsl");
	//sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");

}

void freeOpenGL(GLFWwindow* window) {
	delete sp;
	delete b;
}



void render(GLFWwindow* window, float AD, float WS) {
	//************Tutaj umieszczaj kod rysuj¹cy obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glm::vec3 camPos = glm::vec3(AD, 3.0f, WS);
	glm::vec3 camUp = glm::vec3(0.0f,1.0f,0.0f);

	glm::mat4 M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierz¹ jednostkow¹
	glm::mat4 V = glm::lookAt(camPos, camPos + camFront, camUp); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania

	sp->use();
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	b->draw();

	glfwSwapBuffers(window); 
}

int main(void) {
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "Nie mo¿na zainicjwaæ GLFW\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(800, 800, "Galeria sztuk pieknych we Wiedniu", NULL, NULL);

	if (!window) {
		fprintf(stderr, "Nie mozna otworzyc okna\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW\n");
		exit(EXIT_FAILURE);
	}

	init(window);

	float rotate = 0;
	float AD= 0;
	float WS = 0;
	glfwSetTime(0); 
	while (!glfwWindowShouldClose(window))
	{
		AD += speed_y * glfwGetTime(); 
		WS += speed_x * glfwGetTime(); 
		glfwSetTime(0);
		render(window, AD, WS); 
		glfwPollEvents(); 
	}

	freeOpenGL(window);

	glfwDestroyWindow(window);
	glfwTerminate(); 
	exit(EXIT_SUCCESS);

}