#include "../include.h"

Minerva::Modeling::Mesh::Mesh(
	std::vector<Minerva::Modeling::Vertex> _vertices,
	std::vector<unsigned int> _indices,
	std::vector<std::string> _textures,
	Minerva::System::Renderer* renderer
) {
	vertices = _vertices;
	indices  = _indices;
	textures = _textures;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// vertices
	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

	glBindVertexArray(0);

	for (std::string i : textures) {
		renderer->GetTexture(i);
	}
}

void Minerva::Modeling::Mesh::BindTextures(Minerva::System::Renderer* renderer, Minerva::Component::Renderer* renderer_component) {
	unsigned int diffuse_ct = 1;
	unsigned int specular_ct = 1;

	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		Texture* tex = renderer->GetTexture(textures[i]);

		std::string name;

		switch (tex->type) {
		case TEXTURE_DIFFUSE:  name = "d" + std::to_string(diffuse_ct);  break;
		case TEXTURE_SPECULAR: name = "s" + std::to_string(specular_ct); break;
		}

		renderer_component->program->Set(name.c_str(), i);
		glBindTexture(GL_TEXTURE_2D, tex->id);
	}
}


void Minerva::Modeling::Mesh::Render(Minerva::System::Renderer* renderer, Minerva::Component::Renderer* renderer_component) {
	BindTextures(renderer, renderer_component);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Minerva::Modeling::Mesh::RenderInstanced(Minerva::System::Renderer* renderer, Minerva::Component::Renderer* renderer_component, int num_instances) {
	BindTextures(renderer, renderer_component);

	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0, num_instances);
	glBindVertexArray(0);
}