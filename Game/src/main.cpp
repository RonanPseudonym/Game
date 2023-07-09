#include "../minerva/include.h"

std::vector<float> cube = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

int main() {
	Minerva::Engine engine;
	/*auto renderer = (Minerva::System::Renderer*)engine.AddSystem(new Minerva::System::Renderer());
	engine.AddSystem(new Minerva::System::Controller::Noclip());*/
	engine.AddSystem(new Minerva::System::Server(12345));
	engine.AddSystem(new Minerva::System::Client(12345, "127.0.0.1"));
	engine.Initialize();

	/*Minerva::Modeling::CustomModel model;
	model.AddMeshFromVertexTextureArray(cube, { "d.crate.png" }, renderer);

	renderer->RegisterModel("cube", &model);

	Minerva::Component::Renderer* r = new Minerva::Component::Renderer(
		"cube",
		"v.ibasic:f.basic",
		renderer
	);

	engine.AddPrototype("crate", Minerva::Prototype({
		new Minerva::Component::Transform(),
		new Minerva::Component::Instance(r)
	}));

	for (int i = 0; i < 1000000; i++) {
		unsigned int entity = engine("crate");
		auto transform = Minerva::Get<Minerva::Component::Transform>(&engine, entity);

		transform->position.x = Minerva::Random::SplitRange(200.0);
		transform->position.y = Minerva::Random::SplitRange(200.0);
		transform->position.z = Minerva::Random::SplitRange(200.0);

		transform->rotation.x = Minerva::Random::Range(1.0);
		transform->rotation.y = Minerva::Random::Range(1.0);
		transform->rotation.z = Minerva::Random::Range(1.0);

		transform->angle      = Minerva::Random::Range(360.0);

		double scale = Minerva::Random::Range(0.3, 1.5);
		transform->scale.x = scale;
		transform->scale.y = scale;
		transform->scale.z = scale;
	}

	unsigned int cam = renderer->InstantiateCamera(&engine);
	engine.AddComponent(cam, new Minerva::Component::Noclip());
	*/

	engine.Cycle();

	return 0;
}