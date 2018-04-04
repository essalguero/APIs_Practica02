#include "Camera.h"
#include "State.h"

Camera::Camera()
{

}

void Camera::prepare()
{
	State::projectionMatrix = this->projectionMatrix;
	
	glm::mat4 viewMatrix;

	viewMatrix[3] = glm::vec4(-position, 1.0f);

	State::viewMatrix = viewMatrix;

	glViewport(viewPort.s, viewPort.t, viewPort.p, viewPort.q);
	glScissor(viewPort.s, viewPort.t, viewPort.p, viewPort.q);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}