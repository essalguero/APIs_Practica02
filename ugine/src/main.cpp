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
#include "Model.h"
#include "Camera.h"
#include "World.h"

#include "State.h"

#define FULLSCREEN false

const float ROTATION_SPEED = 32.0f;

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


	// Store the Shader in the global object State
	State::defaultShader = Shader::create(readString("../data/shader.vert"), readString("../data/shader.frag"));

	if (strcmp(State::defaultShader->getError(), "") != 0)
	{
		cout << State::defaultShader->getError() << endl;
		return -1;
	}

	//State::defaultShader->use();


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


	shared_ptr<Buffer> bufferDatos = Buffer::create(vertices, indices);
	if (strcmp(bufferDatos->getError(), "") != 0)
	{
		cout << bufferDatos->getError() << endl;
		return -1;
	}

	shared_ptr<Mesh> triangleMesh = make_shared<Mesh>();
	Model triangleModel(triangleMesh);
	triangleMesh->addBuffer(bufferDatos);

	shared_ptr<Camera> camera = make_shared<Camera>();
	camera->setPosition(glm::vec3(0.0f, 0.0f, 6.0f));
	camera->setClearColor(glm::vec3(0, 0, 0));

	World world;
	//world.addEntity(std::dynamic_pointer_cast<Entity, Camera>(camera));
	world.addEntity(camera);

	
	float anguloRotacionRads = glm::radians(ROTATION_SPEED);
	glm::vec3 scaleVector(1.0f, 1.0f, 1.0f);
	glm::vec3 rotationVector(0.0f, 0.0f, 0.0f);

	vector<shared_ptr<Model>> trianglesVector;

	for (int x = -3; x <= 3; x += 3) {
		for (int z = 0; z >= -6; z -= 3) {
			
			shared_ptr<Model> triangle = make_shared<Model>(triangleMesh);
			triangle->setScale(scaleVector);
			triangle->setRotation(rotationVector);
			triangle->setPosition(glm::vec3(static_cast<float>(x), 0.0f, static_cast<float>(z)));

			world.addEntity(triangle);
			trianglesVector.push_back(triangle);
		}
	}

	// Bucle principal
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

		// Update viewport in case the screen has been resized
		camera->setViewport(glm::ivec4(0, 0, screenWidth, screenHeight));

		// Set projection matrix in case the screen has been resized
		glm::mat4 projectionMatrix = glm::perspective(45.0f, 
			static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);
		camera->setProjection(projectionMatrix);

		// Rotar todos los triangulos
		for (auto triangle : trianglesVector)
		{
			triangle->setRotation(glm::vec3(0.0f, anguloRotacionRads, 0.0f));
		}

		// Crear matriz de vista
		//glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// Transformaciones para calcular la matriz del modelo

		// Escala
		// No se esta realizando escalado de objetos

		// Matriz de Rotacion
		//glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), anguloRotacionRads, glm::vec3(0.0f, 1.0f, 0.0f));
		//
		//triangleModel.setRotation()

		// La posición varía segun cada objeto.
		// Se calcula una matriz de traslación distinta para cada objeto
		/*for (int x = -3; x <= 3; x += 3) {
			for (int z = 0; z >= -6; z -= 3) {

				// Matriz de traslacion
				glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(static_cast<float>(x), 0.0f, static_cast<float>(z)));
				//
				glm::vec3 translationVector = glm::vec3(static_cast<float>(x), 0.0f, static_cast<float>(z));
				triangleModel.setPosition(translationVector);

				// Store the needed Matrices in the class State
				State::modelMatrix = translationMatrix * rotationMatrix;
				State::viewMatrix = viewMatrix;
				State::projectionMatrix = projectionMatrix;

				//Draw the object
				//triangleMesh->draw();
				triangleModel.draw();
			}
		}*/

		// Draw the objects
		world.update(deltaTime);
		world.draw();

		// update swap chain & process events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	return 0;
}