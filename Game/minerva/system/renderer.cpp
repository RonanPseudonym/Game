#include "../include.h"

bool size_changed = false;
int changed_w, changed_h;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) { // maybe useless idk
	glViewport(0, 0, width, height);
	size_changed = true;
	
	changed_w = width;
	changed_h = height;
}

void Minerva::System::Renderer::SetFrustum(float fov) {
	proj = glm::perspective(glm::radians(fov), (float)window_width / (float)window_height, 0.1f, 300.0f);
}

void Minerva::System::Renderer::OnInitialize(Minerva::Engine* engine) {
	InitGLFW  ();
	InitGLAD  ();
	InitOpenGL();
	InitSTBI  ();

	SetFrustum(45.0f);
}


void Minerva::System::Renderer::Cycle2(Minerva::Engine* engine) {
	if (glfwGetWindowAttrib(window, GLFW_ICONIFIED)) {
		if (glfwWindowShouldClose(window)) engine->Terminate();
		glfwPollEvents();

		return;
	}

	if (glfwWindowShouldClose(window)) engine->Terminate();

	if (size_changed) {
		window_width = changed_w;
		window_height = changed_h;

		SetFrustum(45);

		size_changed = false;
	}

	auto camera_pos = Get<Minerva::Component::Camera>(engine, cameras[camera]);
	glm::mat4 camera = proj * glm::lookAt(camera_pos->pos, camera_pos->pos + camera_pos->front, camera_pos->up);

	for (auto entity : engine->Components("renderer")) {
		Minerva::Component::Transform* t = Get<Minerva::Component::Transform>(engine, entity.first);
		Minerva::Component::Renderer* r = (Minerva::Component::Renderer*)entity.second;
		glm::mat4 transform = glm::mat4(1.0f);

		transform = glm::translate(transform, t->position);
		if (t->angle != 0) transform = glm::rotate(transform, glm::radians(t->angle), t->rotation);
		transform = glm::scale(transform, t->scale);

		r->program->Activate();

		r->program->Set("transform", transform);
		r->program->Set("camera", camera);

		r->model->Render(this, r);
	}

	for (auto instance_group : instances) {
		int amount = instance_group.second.size();
		/*glm::mat4* translations = new glm::mat4[amount];

		for (int i = 0; i < amount; i++) {
			Minerva::Component::Transform* transform = instance_group.second[i];
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, transform->position);
			if (transform->angle != 0) model = glm::rotate(model, glm::radians(transform->angle), transform->rotation);
			model = glm::scale(model, transform->scale);

			translations[i] = model;
		}*/

		Minerva::Component::Renderer* renderer = instance_group.first;

		/*unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &translations[0], GL_STATIC_DRAW);

		for (int i = 0; i < renderer->model->meshes.size(); i++) {
			glBindVertexArray(renderer->model->meshes[i].VAO); // TODO: fix

			std::size_t vec4_size = sizeof(glm::vec4);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(1 * vec4_size));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(2 * vec4_size));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(3 * vec4_size));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}*/

		renderer->program->Activate();
		renderer->program->Set("camera", camera);

		renderer->model->RenderInstanced(this, renderer, amount);
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Minerva::System::Renderer::BuildInstanceBuffer(Minerva::Engine* engine) {
	for (auto entity : engine->Components("instance")) {
		instances[((Minerva::Component::Instance*)entity.second)->instance_renderer].push_back(
			Get<Minerva::Component::Transform>(engine, entity.first)
		);
	}

	for (auto instance_group : instances) {
		int amount = instance_group.second.size();
		glm::mat4* translations = new glm::mat4[amount];

		for (int i = 0; i < amount; i++) {
			Minerva::Component::Transform* transform = instance_group.second[i];
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, transform->position);
			if (transform->angle != 0) model = glm::rotate(model, glm::radians(transform->angle), transform->rotation);
			model = glm::scale(model, transform->scale);

			translations[i] = model;
		}

		Minerva::Component::Renderer * renderer = instance_group.first;

		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &translations[0], GL_STATIC_DRAW);

		for (int i = 0; i < renderer->model->meshes.size(); i++) {
			glBindVertexArray(renderer->model->meshes[i].VAO); // TODO: fix

			std::size_t vec4_size = sizeof(glm::vec4);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(1 * vec4_size));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(2 * vec4_size));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4_size, (void*)(3 * vec4_size));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}
	}
}

void Minerva::System::Renderer::OnSetup(Minerva::Engine* engine) {
	BuildInstanceBuffer(engine);
}

void Minerva::System::Renderer::Cycle1(Minerva::Engine* engine) {
	glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Minerva::System::Renderer::OnTerminate(Minerva::Engine* engine) {
	glfwTerminate();
}

void GLFWErrorCallback(int error, const char* msg) {
	Minerva::Debug::Console::Error(("GLFW ERROR " + std::to_string(error) + " " + msg).c_str());
}

void Minerva::System::Renderer::InitGLFW() {
	if (glfwInit() == GLFW_FALSE) Debug::Console::FatalError("Failed to initialize GLFW");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_AUTO_ICONIFY,   GLFW_TRUE);

	glfwSetErrorCallback(GLFWErrorCallback);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	window_width  = mode->width;
	window_height = mode->height;

	window = glfwCreateWindow(window_width, window_height, prefs.window_title, monitor, NULL);
	if (window == NULL) {
		glfwTerminate();
		Debug::Console::FatalError("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
}

void Minerva::System::Renderer::InitGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		Debug::Console::FatalError("Failed to load OpenGL by way of GLAD");
	}
}

void Minerva::System::Renderer::InitOpenGL() {
	glViewport(0, 0, window_width, window_height);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

void Minerva::System::Renderer::InitSTBI() {
	stbi_set_flip_vertically_on_load(true);
}

unsigned int Minerva::System::Renderer::InstantiateCamera(Minerva::Engine* engine) {
	unsigned int id = engine->Instantiate();
	engine->AddComponent(id, new Minerva::Component::Camera());

	cameras.push_back(id);

	return id;
}

Minerva::Shading::Shader* Minerva::System::Renderer::GetShader(std::string name) {
	try {
		return &shaders.at(name);
	} catch (const std::out_of_range&) {
		GLint shader_type;
		switch (name[0]) {
			case 'f': shader_type = GL_FRAGMENT_SHADER; break;
			case 'v': shader_type = GL_VERTEX_SHADER;   break;
		}

		shaders.insert({ name, Minerva::Shading::Shader(name, shader_type) });

		return &shaders.at(name);
	}
}

Minerva::Shading::Program* Minerva::System::Renderer::GetProgram(std::string name) {
	try {
		return &programs.at(name);
	} catch (const std::out_of_range&) {
		programs.insert(
			{
				name,
				Minerva::Shading::Program(
					this,
					name.substr(0, name.find(":")).c_str(),
					name.substr(name.find(":") + 1, name.length()).c_str()
				)
			}
		);

		return &programs.at(name);
	}
}

Minerva::Modeling::Texture* Minerva::System::Renderer::GetTexture(std::string name) {
	try {
		return &textures.at(name);
	}
	catch (const std::out_of_range&) {
		Minerva::Modeling::TextureType texture_type;
		switch (name[0]) {
			case 'd': texture_type = Minerva::Modeling::TEXTURE_DIFFUSE;  break;
			case 'v': texture_type = Minerva::Modeling::TEXTURE_SPECULAR; break;
		}

		textures.insert(
			{
				name,
				Minerva::Modeling::Texture(
					name,
					texture_type
				)
			}
		);

		return &textures.at(name);
	}
}

Minerva::Modeling::ModelBase* Minerva::System::Renderer::GetModel(std::string name) {
	try {
		return models.at(name);
	}
	catch (const std::out_of_range&) {
		// TODO: load model
	}
}

void Minerva::System::Renderer::RegisterModel(std::string name, Modeling::ModelBase* model) {
	models[name] = model;
}

void Minerva::System::Renderer::RemoveModel(std::string name) {
	models.erase(name);
}