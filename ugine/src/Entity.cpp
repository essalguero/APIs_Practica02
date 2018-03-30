#include "Entity.h"

Entity::Entity()
{

}

const glm::vec3& Entity::getPosition() const
{
	return position;
}

void Entity::setPosition(const glm::vec3& pos)
{
	position = pos;
}

const glm::vec3& Entity::getRotation() const // puede ser quat
{
	return rotation;
}

const void Entity::setRotation(const glm::vec3& rot) // puede ser quat
{
	rotation = rot;
}

const glm::vec3& Entity::getScale() const
{
	return scale;
}

void Entity::setScale(const glm::vec3& scale)
{
	this->scale = scale;
}

void Entity::move(const glm::vec3& vec)
{
	position += rotation * vec;
}