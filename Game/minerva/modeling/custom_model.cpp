#include "../include.h"

void Minerva::Modeling::CustomModel::AddMesh(
	std::vector<Minerva::Modeling::Vertex> vertices,
	std::vector<unsigned int> indices,
	Minerva::System::Renderer* renderer) {

	meshes.push_back(Minerva::Modeling::Mesh(vertices, indices, std::vector<std::string>(), renderer));
}

void Minerva::Modeling::CustomModel::AddMesh(
	std::vector<Minerva::Modeling::Vertex> vertices,
	std::vector<unsigned int> indices,
	std::vector<std::string> textures,
	Minerva::System::Renderer* renderer) {

	meshes.push_back(Minerva::Modeling::Mesh(vertices, indices, textures, renderer));
}

void Minerva::Modeling::CustomModel::AddMeshFromVertexTextureArray(std::vector<float> data, std::vector<std::string> textures, Minerva::System::Renderer* renderer) {
	std::vector<Minerva::Modeling::Vertex> vertices;
	std::vector<unsigned int> indices;
	int i = 0;
	int j = 0;
	while (i < data.size()) {
		indices.push_back(j ++);
		indices.push_back(j ++);
		indices.push_back(j ++);

		vertices.push_back({
			glm::vec3(
				data[i++],
				data[i++],
				data[i++]
			),
			glm::vec3(),
			glm::vec2(
				data[i++],
				data[i++]
			)
		});
	}

	meshes.push_back(Minerva::Modeling::Mesh(vertices, indices, textures, renderer));
}