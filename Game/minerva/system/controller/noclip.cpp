#include "../../include.h"

void Minerva::System::Controller::Noclip::OnMouse(Minerva::Engine* engine, double xpos, double ypos) {
	if (first_mouse) // initially set to true
	{
		lastx = xpos;
		lasty = ypos;
		first_mouse = false;
	}

	float xoffset = xpos - lastx;
	float yoffset = lasty - ypos; // reversed since y-coordinates range from bottom to top
	lastx = xpos;
	lasty = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);
}

void Minerva::System::Controller::Noclip::OnInput(Minerva::Engine* engine) {
	GLFWwindow* window = engine->template System<Minerva::System::Renderer>("renderer")->window;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 10.0f * engine->delta_time;
	} else {
		speed = 4.0f * engine->delta_time;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		pos += speed * front ;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		pos -= speed * front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		pos -= glm::normalize(glm::cross(front, up)) * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		pos += glm::normalize(glm::cross(front, up)) * speed;

	for (auto entity : engine->Components("noclip")) {
		Minerva::Component::Camera* c = engine->template Component<Minerva::Component::Camera>(entity.first, "camera");
		c->pos   = pos;
		c->up    = up;
		c->front = front;
	}
}