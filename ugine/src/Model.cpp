#include "Model.h"
#include "State.h"

#include "common.h"

Model::Model(const std::shared_ptr<Mesh>& mesh)
{
	this->mesh = mesh;
}

void Model::draw()
{
	glm::quat rotationQuaternion = glm::quat(rotation);

	// Generate the matrices to be used to calculate the Model Matrix
	mat4 translationMatrix = glm::translate(glm::mat4(), position);
	mat4 rotationMatrix = glm::rotate(mat4(), 
		glm::angle(rotationQuaternion),
		glm::axis(rotationQuaternion));
	mat4 scaleMatrix = glm::scale(mat4(), scale);

	// Apply matrices
	mat4 modelMatrix = rotationMatrix * scaleMatrix;
	modelMatrix = translationMatrix * modelMatrix;

	//Set the matrix as the matrix to use for drawing
	State::modelMatrix = modelMatrix;
	
	// Draw the mesh
	mesh->draw();
}