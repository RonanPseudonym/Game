#include "../minerva/include.h"

int main() {
	Minerva::Engine engine;

	engine.AddSystem(new Minerva::System::Server(12345));
	engine.Initialize();

	engine.Cycle();

	return 0;
}