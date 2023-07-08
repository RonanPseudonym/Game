#include "../include.h"

Minerva::Shading::Program::Program(Minerva::System::Renderer* renderer, const char* _vertex, const char* _fragment) {
	vertex   = renderer->GetShader(_vertex);
	fragment = renderer->GetShader(_fragment);

	int success;
	char error_log[512];

	id = glCreateProgram();
	glAttachShader(id, vertex->id);
	glAttachShader(id, fragment->id);

	glLinkProgram(id); 

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, error_log);
		Minerva::Debug::Console::Error((std::string("Shader program linking error: ") + error_log).c_str());
	}
}

GLint Minerva::Shading::Program::LookupUniform(const char* name) {
	GLint loc = glGetUniformLocation(id, name);
	if (loc != -1) return loc;

	Minerva::Debug::Console::Warning("Uniform not found in shader program");
}

void Minerva::Shading::Program::Set(const char* name, bool val) {
	glUniform1i(LookupUniform(name), (int)val);
}

void Minerva::Shading::Program::Set(const char* name, int val) {
	glUniform1i(LookupUniform(name), val);
}

void Minerva::Shading::Program::Set(const char* name, float val) {
	glUniform1f(LookupUniform(name), val);
}

void Minerva::Shading::Program::Set(const char* name, glm::mat4 val) {
	glUniformMatrix4fv(LookupUniform(name), 1, GL_FALSE, glm::value_ptr(val));
}

void Minerva::Shading::Program::Activate() {
	glUseProgram(id);
}