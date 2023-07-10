#include "../minerva/include.h"

int main() {
	Minerva::Engine engine;

	engine.AddSystem(new Minerva::System::Client(12345, "127.0.0.1"));
	engine.Initialize();

	engine.Cycle();

	return 0;
}