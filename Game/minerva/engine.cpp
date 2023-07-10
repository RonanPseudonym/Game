#include "include.h"

Minerva::Engine::Engine() {
	current_id = 0;

	std::srand(std::time(nullptr));
}

void Minerva::Engine::Initialize() {
	for (auto system : systems) {
		system.second->OnInitialize(this);
	}
}

void ThreadedLoop(Minerva::System::Base* system, int fn, int fps, Minerva::Engine* engine) {
	double wait = ((double)fps / 60.0);

	double delta_time = 0;
	double start;

	while (!engine->threads_should_terminate) {
		start = glfwGetTime();

		switch (fn) {
			case 1: system->Process1(engine, delta_time); break;
			case 2: system->Process2(engine, delta_time); break;
			case 3: system->Process3(engine, delta_time); break;
			case 4: system->Process4(engine, delta_time); break;
		}
		

		if (fps) {
			double active_wait = wait - (glfwGetTime() - start);
			if (active_wait > 0) std::this_thread::sleep_for(std::chrono::milliseconds((int)(active_wait * 1000)));
		}

		delta_time = glfwGetTime() - start;
	}
}

void Minerva::Engine::Cycle() {
	for (auto system : systems) {
		system.second->OnSetup(this);
	}

	bool has_renderer = systems.count("renderer");
	GLFWwindow* window;
	if (has_renderer) window = Get<Minerva::System::Renderer>(this)->window; // this can be optimized prolly
	else {
		window = NULL;
		glfwInit();
	}

	for (;;) {
		double before = glfwGetTime();
		// pre-cycle

		if (has_renderer) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			for (auto system : systems) {
				system.second->OnMouse(this, xpos, ypos);
			}

			for (auto system : systems) {
				system.second->OnInput(this);
			}
		}

		// update

		for (auto system : execution_order) {
			switch (system.second) {
				case 1: system.first->Cycle1(this); break;
				case 2: system.first->Cycle2(this); break;
				case 3: system.first->Cycle3(this); break;
				case 4: system.first->Cycle4(this); break;
			}
		}

		delta_time = (glfwGetTime() - before);
	}
}

void Minerva::Engine::Terminate() {
	threads_should_terminate = true;

	for (auto system : systems) {
		system.second->OnTerminate(this);
	}

	// TODO: flesh out
	exit(0);
}

Minerva::Engine::~Engine() {
	Terminate();
}

unsigned int Minerva::Engine::Instantiate() {
	return current_id++;
}

unsigned int Minerva::Engine::Instantiate(std::string prototype) {
	unsigned int id = Instantiate();

	for (auto cmpt : GetPrototype(prototype)->components)
		Minerva::Engine::AddComponent(id, cmpt->Clone());

	return id;
}


unsigned int Minerva::Engine::operator()() {
	return Instantiate();
}

unsigned int Minerva::Engine::operator()(std::string prototype) {
	return Instantiate(prototype);
}

void Minerva::Engine::Destroy(unsigned int entity) {
	for (auto& i : components) {
		delete i.second[entity];
		i.second.erase(entity);
	}
}

void Minerva::Engine::AddComponent(
	unsigned int entity, 
	Minerva::Component::Base* component) 
{
	components[component->Name()][entity] = component;
}

void Minerva::Engine::RemoveComponent(unsigned int entity, std::string name) {
	delete components[name][entity];
	components[name].erase(entity);
}

void Minerva::Engine::Systems(std::vector<System::Base*> s) {
	for (auto system : s) {
		systems[system->Name()] = system;
	}
}

void Minerva::Engine::ExecutionOrder(std::vector<std::pair<std::string, unsigned int>> order) {
	for (auto i : order) {
		execution_order.push_back(std::make_pair(GetRawSystem(i.first), i.second));
	}
}

void Minerva::Engine::Spawn(std::vector<std::pair<std::pair<std::string, unsigned int>, unsigned int>> t) {
	for (auto i : t) {
		new std::thread(ThreadedLoop, GetRawSystem(i.first.first), i.first.second, i.second, this);
	}
}

Minerva::Prototype* Minerva::Engine::AddPrototype(std::string name, Minerva::Prototype p) {
	prototypes[name] = p;
	return &prototypes[name];
}
void Minerva::Engine::RemovePrototype(std::string name) {
	prototypes.erase(name);
}

Minerva::Prototype* Minerva::Engine::GetPrototype(std::string name) {
	return &prototypes[name];
}

Minerva::Component::Base* Minerva::Engine::GetRawComponent(unsigned int id, std::string name) {
	return components[name][id];
}

Minerva::System::Base* Minerva::Engine::GetRawSystem(std::string name) {
	return systems[name];
}

/*template <class T>
T* Minerva::Engine::System(std::string system_name) {
	return (T*)systems[system_name];
}

template <class T>
T* Minerva::Engine::Component(int entity, std::string name) {
	Minerva::Component::Base* c = components[name][entity];
	if (!c) Minerva::Debug::Console::Error((std::string("Unable to get ") + std::to_string(entity) + ":" + name).c_str());

	return (T*)c;
}

template <class T>
std::unordered_map<unsigned int, T*> Minerva::Engine::Components(std::string component_name) {
	return components[component_name];
}*/