
#pragma once

#include "Renderer.h"

class Program {
public:
	virtual void load(Renderer&) {}
	virtual void update(Renderer&) {}
	virtual void draw(Renderer&) {}
	virtual void drawImGui(Renderer&) {}

	virtual void mousePressed(Renderer&, SDL_MouseButtonEvent) {}
};
