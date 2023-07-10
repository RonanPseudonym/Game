#include "../minerva/include.h"

int main() {
	Minerva::Engine engine;

	engine.Systems({
		new Minerva::System::Client(12345, "127.0.0.1")
	});
	
	engine.ExecutionOrder({
		{"client", 1}
	});

	engine.Initialize();


	engine.Spawn({
		{{"client", 1}, 0}
	});

	engine.Cycle();

	return 0;
}