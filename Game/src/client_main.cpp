#include "../minerva/include.h"

int main() {
	Minerva::Engine engine;

	engine.Systems({
		new Minerva::System::Client(12345, "127.0.0.1"),
		new Minerva::System::Renderer(),
		new Minerva::System::Controller::Noclip()
	});
	
	engine.ExecutionOrder({
		{"client", 1},
		{"renderer", 1},
		{"renderer", 2}
	});

	engine.Initialize();

	unsigned int cam = Minerva::Get<Minerva::System::Renderer>(&engine)->InstantiateCamera(&engine);
	engine.AddComponent(cam, new Minerva::Component::Noclip());

	engine.Spawn({
		{{"client", 1}, 0}
	});

	engine.Cycle();

	return 0;
}