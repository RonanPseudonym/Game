#include "../include.h"

/*Minerva::Modeling::FileModel::FileModel(std::string _file_name) {
	ModelBase();

	file_name = _file_name;
}

void Minerva::Modeling::FileModel::Load(Minerva::System::RendererPrefs& prefs) {
	path = prefs.resource_path + "/" + prefs.model_directory;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile((path + "/" + file_name).c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Minerva::Debug::Console::Error((std::string("Assimp: ") + importer.GetErrorString()).c_str());
		return;
	}

	ProcessNode(scene->mRootNode, scene);
}

void Minerva::Modeling::FileModel::ProcessNode(aiNode* node, const aiScene* scene) {
	// process meshes
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	// recurse
	for (int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

unsigned int Minerva::Modeling::FileModel::TextureFromFile(const char* file_name) {
	std::string p = path + "/" + file_name;

	unsigned int texture_id;
	glGenTextures(1, &texture_id);

	int width, height, n_components;
	unsigned char* data = stbi_load(p.c_str(), &width, &height, &n_components, 0);

	if (data) {
		GLenum format;
		
		switch (n_components) {
			case 1: format = GL_RED;  break;
			case 3: format = GL_RGB;  break;
			case 4: format = GL_RGBA; break;
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	} else {
		Minerva::Debug::Console::Error("STBI failed to load texture");
	}

	return texture_id;
}

std::vector<Minerva::Modeling::Texture> Minerva::Modeling::FileModel::LoadTextures(
	aiMaterial* mat, 
	aiTextureType type, 
	Minerva::Modeling::TextureType type_enum) 
{
	std::vector<Minerva::Modeling::Texture> textures;

	for (int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		//std::string type_name = Minerva::Modeling::texture_names[type_enum];

		mat->GetTexture(type, i, &str);
		Minerva::Modeling::Texture texture;
		texture.id = TextureFromFile(str.C_Str());
		texture.type = type_enum;
		textures.push_back(texture);
	}

	return textures;
}

Minerva::Modeling::Mesh Minerva::Modeling::FileModel::ProcessMesh(aiMesh *mesh, const aiScene* scene) {
	std::vector<Minerva::Modeling::Vertex>  vertices;
	std::vector<unsigned int>               indices;
	std::vector<Minerva::Modeling::Texture> textures;

	for (int i = 0; i < mesh->mNumVertices; i++) { // vertices
		Minerva::Modeling::Vertex vertex;

		vertex.position = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		vertex.normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);

		if (mesh->mTextureCoords[0]) {
			vertex.texture = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		} else {
			vertex.texture = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (int i = 0; i < mesh->mNumFaces; i++) { // indices
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[i]);
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Minerva::Modeling::Texture> diffuse_maps = LoadTextures(
			material,
			aiTextureType_DIFFUSE,
			Minerva::Modeling::TextureType::TEXTURE_DIFFUSE);
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

		std::vector<Minerva::Modeling::Texture> specular_maps = LoadTextures(
			material,
			aiTextureType_SPECULAR,
			Minerva::Modeling::TextureType::TEXTURE_SPECULAR);
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
	}

	return Mesh(vertices, indices, textures);
}*/