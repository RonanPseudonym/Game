#include "include.h"

Minerva::Engine::Engine() {
	current_id = 0;

	std::srand(std::time(nullptr));
}

void Minerva::Engine::Initialize() {
	for (auto system : on_initialize) {
		system.second->OnInitialize(this);
	}
}

void Minerva::CycleEngineThread(Minerva::Engine* engine, Minerva::System::Base* system, std::string system_name) {
	while (!engine->threads_should_terminate) {
		if (engine->threads_to_complete[system_name]) {
			system->OnCycle(engine);
			engine->threads_to_complete[system_name] = false;
		}
	}
}

void StallUntilThreadsExecuted(Minerva::Engine* engine) {
	for (;;) {
		restart:

		for (auto thread : engine->threads_to_complete) {
			if (thread.second) goto restart; // im sorryyyyyyy
		}

		break;
	}
}

void Minerva::Engine::Cycle() {
	for (auto system : on_first_cycle) {
		system.second->OnFirstCycle(this);
	}

	for (;;) {
		bool has_renderer = systems.count("renderer");
		GLFWwindow* window;
		if (has_renderer) window = Get<Minerva::System::Renderer>(this)->window; // this can be optimized prolly
		else {
			window = NULL;
		}

		double before = glfwGetTime();
		// pre-cycle

		for (auto system : on_precycle) {
			system.second->OnPrecycle(this);
		}

		if (has_renderer) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			for (auto system : on_mouse) {
				system.second->OnMouse(this, xpos, ypos);
			}

			for (auto system : on_input) {
				system.second->OnInput(this);
			}
		}

		// main loop
		for (auto system : threads_to_complete) {
			threads_to_complete[system.first] = true;
		}

		StallUntilThreadsExecuted(this); // wait until yalls done

		// update

		for (auto system : on_update) {
			system.second->OnUpdate(this);
		}

		delta_time = (glfwGetTime() - before);
	}
}

void Minerva::Engine::Terminate() {
	threads_should_terminate = true;

	for (auto system : on_terminate) {
		system.second->OnTerminate(this);
	}

	// TODO: flesh out
	exit(0);
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

Minerva::System::Base* Minerva::Engine::AddSystem(System::Base* system) {
	std::string system_name = system->Name();
	systems[system_name] = system;
	Minerva::System::CallbackRequests cb = system->GetCallbackRequests();

	if (cb.on_initialize)  on_initialize [system_name] = system;
	if (cb.on_update)      on_update     [system_name] = system;
	if (cb.on_precycle)    on_precycle   [system_name] = system;
	if (cb.on_terminate)   on_terminate  [system_name] = system;
	if (cb.on_input)       on_input      [system_name] = system;
	if (cb.on_mouse)       on_mouse      [system_name] = system;
	if (cb.on_first_cycle) on_first_cycle[system_name] = system;

	if (cb.on_cycle) {
		threads[system_name] = new std::thread(CycleEngineThread, this, system, system_name);
		threads_to_complete[system_name] = false;
	}

	return system;
}

void Minerva::Engine::RemoveSystem(std::string system_name) {
	Minerva::System::Base* system = systems[system_name];
	Minerva::System::CallbackRequests cb = system->GetCallbackRequests();

	if (cb.on_initialize)  on_initialize .erase(system_name);
	if (cb.on_update)      on_update     .erase(system_name);
	if (cb.on_precycle)    on_precycle   .erase(system_name);
	if (cb.on_terminate)   on_terminate  .erase(system_name);
	if (cb.on_input)       on_input      .erase(system_name);
	if (cb.on_mouse)       on_mouse      .erase(system_name);
	if (cb.on_first_cycle) on_first_cycle.erase(system_name);

	if (cb.on_cycle) { // TODO: fix
		delete& threads[system_name];
		threads.erase(system_name);
		threads_to_complete.erase(system_name);
	}

	delete system;
	systems.erase(system_name);
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