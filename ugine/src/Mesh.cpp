#include "Mesh.h"
#include "State.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::addBuffer(const std::shared_ptr<Buffer>& buffer, const std::shared_ptr<Shader>& shader)
{
	buffersVector.push_back(buffer);

	if (nullptr != shader)
	{
		shadersVector.push_back(shader);
	}
	else
	{
		shadersVector.push_back(State::defaultShader);
	}
}

size_t Mesh::getNumBuffers() const
{
	return buffersVector.size();
}

const std::shared_ptr<Buffer>& Mesh::getBuffer(size_t index) const
{
	shared_ptr<Buffer> pointer = nullptr;

	if (index >= 0 && index < buffersVector.size())
		return buffersVector.at(index);

	return pointer;
}

std::shared_ptr<Buffer>& Mesh::getBuffer(size_t index)
{
	shared_ptr<Buffer> pointer = nullptr;

	if (index >= 0 && index < buffersVector.size())
		return buffersVector.at(index);

	return pointer;
}

void Mesh::draw()
{
	for (int i = 0; i < buffersVector.size(); ++i)
	{
		shared_ptr<Shader> shader = shadersVector.at(i);

		//Activate the shader
		shader->use();

		// Get location of the mvpMatrix inside the shader
		int matrixLocation = shader->getLocation("mvpMatrix");

		// Calculate the mvpMatrix
		glm::mat4 mvMatrix = State::viewMatrix * State::modelMatrix;
		glm::mat4 mvpMatrix = State::projectionMatrix * mvMatrix;

		// Pass the MVP matrix to the shader program
		shader->setMatrix(matrixLocation, mvpMatrix);

		// Draw the buffer using the shader
		buffersVector.at(i)->draw(shader);

		
	}
}