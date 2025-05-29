#include <SFML/Graphics.hpp>
#include "MainLoop.hpp"

#ifdef WIN32

#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MainLoop mainLoop;
	mainLoop.Run();

	return 0;
}

#else 

int main() {
	MainLoop mainLoop;
	mainLoop.Run();

	return 0;
}

#endif