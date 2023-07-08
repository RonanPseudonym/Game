#include "../include.h"

void __PrettyPrint(const char* tag, const char* text) {
	std::cout << "[" << glfwGetTime() << ":" << tag << "] " << text << std::endl;
}

void Minerva::Debug::Console::Log        (const char* text) { __PrettyPrint("LOG",  text); }
void Minerva::Debug::Console::Warning    (const char* text) { __PrettyPrint("WARN", text); }
void Minerva::Debug::Console::Error      (const char* text) { __PrettyPrint("ERR",  text); }
void Minerva::Debug::Console::FatalError (const char* text) {
	std::cout << "\n========== FATAL ERROR ==========" << std::endl;
	__PrettyPrint("FATAL", text); exit(1); 
}