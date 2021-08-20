
#include "GravitySimulator.h"
#include "Window.h"

int main(int, char**)
{
	GravitySimulator gsim;
	Window window(gsim);

	return window.runMainLoop();
}
