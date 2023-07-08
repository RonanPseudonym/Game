#include "../include.h"
#include <fstream>
#include <sstream>

Minerva::Shading::Shader::Shader(std::string name, GLint _type) {
	type = _type;

    int success;
    char error_log[512];

    std::string buffer = FileToBuffer(name);
    const char* buffer_cstr = buffer.c_str();

    id = glCreateShader(type);
    glShaderSource(id, 1, &buffer_cstr, NULL);
    glCompileShader(id);

    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, error_log);
        Minerva::Debug::Console::Error((std::string("Shader compilation error: ") + error_log).c_str());
    }
}

std::string Minerva::Shading::Shader::FileToBuffer(std::string name) {
    std::string path = "resources/shader/" + name + ".glsl";

    std::string buffer;
    std::ifstream file;
    // ensure ifstream objects can throw exceptions:
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        file.open(path);
        std::stringstream stream;
        // read file's buffer contents into streams
        stream << file.rdbuf();
        // close file handlers
        file.close();
        // convert stream into string
        buffer = stream.str();
    }
    catch (std::ifstream::failure e)
    {
        Minerva::Debug::Console::Error("Unable to read shader file");
    }

    return buffer;
}