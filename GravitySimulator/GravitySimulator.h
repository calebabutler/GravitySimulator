
#pragma once

#include <vector>

#include "Mass.h"
#include "Program.h"

class GravitySimulator : public Program {
public:
	explicit GravitySimulator();

	void update(Renderer&) final;
	void draw(Renderer&) final;
	void drawImGui(Renderer&) final;

	void mousePressed(Renderer&, SDL_MouseButtonEvent) final;

private:
	std::vector<Mass> m_masses;
	const ImVec4* m_nextMassColor;

	bool m_isWaitingForVelocity;
	bool m_doCircularOrbit;

	float m_newMassMass;

	Vector2 getAcceleration(float x, float y, int ignoreIndex);
	void drawMasses(Renderer&);
	void setNextMassColor();

	void drawImGuiOverlay(Renderer&);
	void drawImGuiExistingMasses(Renderer&);
	void drawImGuiNewMasses(Renderer&);
};
