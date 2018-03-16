#include "Buffer.h"


Buffer::~Buffer() {
	glDeleteBuffers(2, buffer);
}
 
void Buffer::draw(const Shader& shader) const {

	// Volver a enlazar los buffers de datos antes de usarlos
	// Los datos ya se cargaron en los buffers en el Constructor
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);

	shader.setupAttribs();

	// draw triangle
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, nullptr);
}
