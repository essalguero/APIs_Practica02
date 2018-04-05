#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

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
#include "Entity.h"
#include "Model.h"
#include "Camera.h"
#include "World.h"

#include "State.h"

#define FULLSCREEN false

const float ROTATION_SPEED = 32.0f;
const float ROTATION_SPEED_RADS = glm::radians(ROTATION_SPEED);

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


// This method creates all the models and add them to the world
int createModelsInWorld(World & world)
{
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
		return 0;
	}

	shared_ptr<Mesh> triangleMesh = make_shared<Mesh>();
	Model triangleModel(triangleMesh);
	triangleMesh->addBuffer(bufferDatos);

	glm::vec3 scaleVector(1.0f, 1.0f, 1.0f);
	glm::vec3 rotationVector(0.0f, 0.0f, 0.0f);

	// create the triangles in the scene
	for (int x = -3; x <= 3; x += 3) {
		for (int z = 0; z >= -6; z -= 3) {

			shared_ptr<Model> triangle = make_shared<Model>(triangleMesh);
			triangle->setScale(scaleVector);
			triangle->setRotation(rotationVector);
			triangle->setPosition(glm::vec3(static_cast<float>(x), 0.0f, static_cast<float>(z)));

			//Add them to the world object
			world.addEntity(triangle);
		}
	}

	return 1;
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

	// If there  was any error on the generation of the sharder, raise an error
	if (strcmp(State::defaultShader->getError(), "") != 0)
	{
		cout << State::defaultShader->getError() << endl;
		return -1;
	}

	// Generate the world
	World world;

	// Generate a camera and store it in the world
	shared_ptr<Camera> camera = make_shared<Camera>();
	camera->setPosition(glm::vec3(0.0f, 0.0f, 6.0f));
	camera->setClearColor(glm::vec3(0, 0, 0));
	world.addEntity(camera);

	// Generate the objects in the world
	if (!createModelsInWorld(world))
	{
		cout << "Error creating the Model objects in the world" << endl;
		return -1;
	}

	// create a cuaternion with the  
	glm::quat rotationQuat = angleAxis(ROTATION_SPEED_RADS,
		glm::vec3(0.0f, 1.0f, 0.0f));

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

		
		// Update the objects in the world
		for (int i = 0; i < world.getNumEntities(); ++i) {

			std::shared_ptr<Entity> currentEntity = world.getEntity(i);

			//Only Model objects must be displayed
			std::shared_ptr<Model> currentModel = std::dynamic_pointer_cast<Model>(currentEntity);
			if (currentModel != nullptr)
			{
				// get the current quaternion of the object
				glm::quat rotationQuaternion = currentModel->getRotationQuat();

				// Calculate the new quaternion
				currentModel->setRotation(glm::slerp(rotationQuaternion, 
					rotationQuat * rotationQuaternion, deltaTime));
				
			}
		}

		// Draw the objects
		world.update(deltaTime);
		world.draw();

		// update swap chain & process events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	return 0;
}