#include "../include.h"

void Minerva::Modeling::ModelBase::Render(
	Minerva::System::Renderer*    renderer, 
	Minerva::Component::Renderer* renderer_component
) {
	for (Mesh i : meshes) {
		i.Render(renderer, renderer_component);
	}
}

void Minerva::Modeling::ModelBase::RenderInstanced(
	Minerva::System::Renderer* renderer,
	Minerva::Component::Renderer* renderer_component,
	int num_instances
) {
	for (Mesh i : meshes) {
		i.RenderInstanced(renderer, renderer_component, num_instances);
	}
}