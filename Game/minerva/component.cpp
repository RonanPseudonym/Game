#include "include.h"

Minerva::Component::Renderer::Renderer(std::string _model, std::string _program, Minerva::System::Renderer* renderer) {
	model   = renderer->GetModel(_model);
	program = renderer->GetProgram(_program);
}