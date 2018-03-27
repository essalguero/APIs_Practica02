#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

//#include <array>
#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>

#include <memory>


#include "../lib/glew/glew.h"
#include "../lib/glfw3/glfw3.h"
#include "common.h"
#include "Shader.h"
#include "Vertex.h"
#include "Buffer.h"
#include "Mesh.h"

#include "State.h"

#define FULLSCREEN false


std::string readString(const char* filename) {
	std::ifstream f(filename, std::ios_base::binary);
	std::stringstream ss;
	ss << f.rdbuf();
	return ss.str();
}


int init() {
	

	// init glew
	if (glewInit()) {
		std::cout << "could not initialize glew" << std::endl;
		return -1;
	}

	// enable gl states
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	return 0;

}



int main(int, char**) {


	if (glfwInit() != GLFW_TRUE) {
		std::cout << "could not initalize glfw" << std::endl;
		return -1;
	}
	atexit(glfwTerminate);

	// create window
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "U-gine", FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (!window) {
		std::cout << "could not create glfw window" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (init())
		return -1;


	//State state;
	
	//state.defaultShader = Shader::create(readString("../data/shader.vert"), readString("../data/shader.frag"));
	State::defaultShader = Shader::create(readString("../data/shader.vert"), readString("../data/shader.frag"));

	if (strcmp(State::defaultShader->getError(), "") != 0)
	{
		cout << State::defaultShader->getError() << endl;
		return -1;
	}

	State::defaultShader->use();


	// Crear el Buffer que contiene los datos de un triángulo
	vector<Vertex> vertices;
	vector<uint16_t> indices;


	Vertex v1{ glm::vec3(0.0f, 1.0f, 0.0f) };
	Vertex v2{ glm::vec3(-1.0f, -1.0f, 0.0f) };
	Vertex v3{ glm::vec3(1.0f, -1.0f, 0.0f) };

	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);


	shared_ptr<Buffer> bufferDatos = make_shared<Buffer>(vertices, indices);

	Mesh triangleMesh;
	triangleMesh.addBuffer(bufferDatos);
	//Bucle principal

	float anguloRotacionRads = 0.0f;

	

	float lastTime = static_cast<float>(glfwGetTime());
	while ( !glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) ) {
		// update delta time
		float newTime = static_cast<float>(glfwGetTime());
		float deltaTime = newTime - lastTime;
		lastTime = newTime;

		// get updated screen size
		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);

		// report screen size
		std::stringstream ss;
		ss << screenWidth << "x" << screenHeight;
		glfwSetWindowTitle(window, ss.str().c_str());

		// clear screen
		glViewport(0, 0, screenWidth, screenHeight);
		glScissor(0, 0, screenWidth, screenHeight);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Update the rotation angle
		anguloRotacionRads += (glm::radians(32.0f) * deltaTime);

		glm::mat4 mvpMatrix;
		int matrixLocation = State::defaultShader->getLocation("mvpMatrix");


		// Crear matriz de proyeccion
		glm::mat4 projectionMatrix = glm::perspective(45.0f, 
			static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);

		// Crear matriz de vista
		glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 6.0f), 
			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// Matriz Vista Proyeccion
		glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

		// Transformaciones para calcular la matriz del modelo

		// Escala
		// No se esta realizando escalado de objetos

		// Matriz de Rotacion
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), anguloRotacionRads, glm::vec3(0.0f, 1.0f, 0.0f));

		// La posición varía segun cada objeto.
		// Se calcula una matriz de traslación distinta para cada objeto
		for (int x = -3; x <= 3; x += 3) {
			for (int z = 0; z >= -6; z -= 3) {

				// Matriz de traslacion
				glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(static_cast<float>(x), 0.0f, static_cast<float>(z)));

				// Aplicar transformaciones al objeto

				// 1º rotacion, despues traslacion (1º sería la operación de escalado, pero no se aplica en este ejercicio)
				//mvpMatrix = translationMatrix * rotationMatrix;

				// Multiplicar por la matriz de vista y proyeccion para obtener la matriz MVP
				//mvpMatrix = viewProjectionMatrix * mvpMatrix;

				State::modelMatrix = translationMatrix * rotationMatrix;
				State::viewMatrix = viewMatrix;
				State::projectionMatrix = projectionMatrix;

				// Pass the MVP matrix to the shader program
				State::defaultShader->setMatrix(matrixLocation, mvpMatrix);



				//bufferDatos->draw(State::defaultShader);
				triangleMesh.draw();
			}
		}


		// update swap chain & process events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	return 0;
}