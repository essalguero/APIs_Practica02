#pragma once

#include "common.h"

class Entity
{
public:

	Entity();
	
	inline const glm::vec3& getPosition() const;
	void setPosition(const glm::vec3& pos);
	inline const glm::vec3& getRotation() const; // puede ser quat
	const void setRotation(const glm::vec3& rot); // puede ser quat
	inline const glm::vec3& getScale() const;
	void setScale(const glm::vec3& scale);
	void move(const glm::vec3& vec); // Ver en las diapositivas

	// virtual methods that must be redefine in classes inheriting from Entity;
	virtual ~Entity() {};	
	virtual void update(float deltaTime) {};
	virtual void draw() {};

protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

};