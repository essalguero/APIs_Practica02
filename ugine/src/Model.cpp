#include "Model.h"
#include "State.h"


Model::Model(const std::shared_ptr<Mesh>& mesh)
{
	this->mesh = mesh;
}

void Model::update(float deltaTime)
{
	//glm::quat rotationQuaternion = glm::quat(rotation);

	//rotationQuaternion.operator glm::tmat4x4<float, glm::packed_highp>();

	glm::mat3 eulerMatrix(rotation.x, 0.0f, 0.0f, 0.0f, rotation.y, 0.0f, 0.0f, 0.0f, rotation.z);
	glm::tquat<float> rotationQuaternion = glm::toQuat(eulerMatrix);


}


void Model::draw()
{
	glm::mat4 modelMatrix;
	float anguloRotacionRads = 0.0f;

	glm::mat4 scaleMatrix = glm::mat4();

	//glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), anguloRotacionRads, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationMatrix = glm::mat4() * scaleMatrix;
	//glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(static_cast<float>(x), 0.0f, static_cast<float>(z)));
	glm::mat4 translationMatrix = glm::translate(glm::mat4(), position);

	modelMatrix = translationMatrix * rotationMatrix;

	State::modelMatrix = modelMatrix;

	mesh->draw();
}