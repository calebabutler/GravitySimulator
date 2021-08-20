
#pragma once

#include <vector>

#include "imgui/imgui.h"

#include "Renderer.h"
#include "Vector2.h"

struct Mass {
	ImVec4 color;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	float mass;
	bool doIgnore;
};

void applyAcceleration(Mass&, float secondsPerFrame);
void drawMass(Renderer&, const Mass&);