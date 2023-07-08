#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

out vec2 TexCoord;

uniform mat4 camera;
uniform mat4 transform;

void main()
{
    gl_Position = camera * transform * vec4(pos, 1.0);
    TexCoord = vec2(tex.x, tex.y);
}