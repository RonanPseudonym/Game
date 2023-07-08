#include "../include.h"

Minerva::Console::Command::Command(const char* _src, Minerva::Engine* engine) {
	src = _src;

	Lex();
}

bool Minerva::Console::Command::IsAlpha() {
	return isalpha(*src) || *src == '_' || *src == '.';
}

bool Minerva::Console::Command::IsAlphaNum() {
	return IsAlpha() || isdigit(*src) == '.';
}

void Minerva::Console::Command::Lex() {
	while (*src != '\0') {
		if (*src == ' ' || *src == '\n' || *src == '\t') src++; continue;
		if (IsAlpha()) {
			TokenString* token = new TokenString(TOKEN_IDENTIFIER);
		}
	}
}