#include "../minerva/include.h"

int main() {
	Minerva::Engine engine;

	engine.Systems({
		new Minerva::System::Server(12345)
	});

	engine.ExecutionOrder({
		{"server", 1},
		{"server", 2},
	});

	engine.Initialize();

	engine.Spawn({
		{{"server", 1}, 0}
	});

	engine.Cycle(20);

	return 0;
}