#pragma once
#include "../lib/glm/glm.hpp"
#include "../lib/glew/glew.h"

#include <iostream>


using namespace std;

class Shader
{
public:
	Shader(const string & stringVertexShader, const string & stringFragmentShader);

	~Shader();


	// Devuelve el identificador de OpenGL del programa
	inline uint32_t	getId() const { return id; };

	// Obtiene el mensaje de error generado al compilar o enlazar
	inline const char*	getError() const { return error; };

	// Activa el uso de este programa
	inline void	use() const { glUseProgram(id); } ;

	// Activa la escritura de las variables attribute,
	// y especifica su formato
	void setupAttribs() const;

	// Obtiene la localizaci�n de una variable uniform
	int	getLocation(const char* name) const;

	// Da valor a una variable uniform
	void setInt(int loc, int val);
	void setFloat(int loc, float val);
	void setVec3(int loc, const glm::vec3& vec);
	void setVec4(int loc, const glm::vec4& vec);
	void setMatrix(int loc, const glm::mat4& matrix);



private:

	GLuint id{ 0 };

	char error[1024];

	int vposLoc;

	int mvpMatrix;
};

