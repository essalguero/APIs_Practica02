#include "Camera.h"
#include "State.h"

Camera::Camera()
{

}

void Camera::prepare()
{
	State::projectionMatrix = this->projectionMatrix;
	//State::viewMatrix = this->position * this->rotation;

	//glm::mat4 viewMatrix = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 viewMatrix;

	viewMatrix[3] = glm::vec4(-position, 1.0f);

	State::viewMatrix = viewMatrix;

	//glViewport(0, 0, screenWidth, screenHeight);
	glViewport(viewPort.s, viewPort.t, viewPort.p, viewPort.q);
	//glScissor(0, 0, screenWidth, screenHeight);
	glScissor(viewPort.s, viewPort.t, viewPort.p, viewPort.q);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}