#include <SFML/Graphics.hpp>

#pragma comment(lib, "opengl32.lib")

#include "MainLoop.hpp"

int main() {
	MainLoop mainLoop;
	mainLoop.Run();

	return 0;
}