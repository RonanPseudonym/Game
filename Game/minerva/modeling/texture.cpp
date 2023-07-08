#include "../include.h"

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

Minerva::Modeling::Texture::Texture(std::string file, TextureType _type) {
	type = _type;

	int width, height, channels;
	unsigned char* data = stbi_load(("resources/texture/" + file).c_str(), &width, &height, &channels, 0);

	if (data) {
		GLenum format;

		switch (channels) {
		case 1: format = GL_RED;  break;
		case 3: format = GL_RGB;  break;
		case 4: format = GL_RGBA; break;
		}

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		Minerva::Debug::Console::Error("STBI failed to load texture");
	}

	stbi_image_free(data);
}