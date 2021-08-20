
#include <cmath>

#include <sstream>
#include <string>

#include <glad/glad.h>

#include "imgui/imgui.h"

#include "Vector2.h"
#include "Mass.h"

#include "GravitySimulator.h"

// Global Constants
namespace {

	const ImVec4 ORANGE[3] = {
		{1.0f, 0.4f, 0.0f, 1.0f},
		{1.0f, 0.6f, 0.0f, 1.0f},
		{1.0f, 0.8f, 0.0f, 1.0f},
	};

	const ImVec4 YELLOW[3] = {
		{1.0f, 1.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 0.6f, 1.0f},
		{1.0f, 1.0f, 0.8f, 1.0f},
	};

	const ImVec4 GREEN[3] = {
		{0.0f, 0.8f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.2f, 1.0f},
		{0.4f, 1.0f, 0.4f, 1.0f},
	};

};

GravitySimulator::GravitySimulator() : m_nextMassColor(ORANGE), m_isWaitingForVelocity(false), m_newMassMass(100), m_doCircularOrbit(false)
{
	m_masses.reserve(50);
}

void GravitySimulator::update(Renderer& renderer)
{
	for (int i = 0; i < m_masses.size(); i++) {
		m_masses[i].acceleration = getAcceleration(m_masses[i].position.x, m_masses[i].position.y, i);
		applyAcceleration(m_masses[i], 1.0f / renderer.frameRate());
	}
}

void GravitySimulator::draw(Renderer& renderer)
{
	drawMasses(renderer);
}

void GravitySimulator::drawImGui(Renderer& renderer)
{

	drawImGuiExistingMasses(renderer);
	drawImGuiNewMasses(renderer);
	drawImGuiOverlay(renderer);
}

void GravitySimulator::mousePressed(Renderer& renderer, SDL_MouseButtonEvent e)
{
	if (e.type == SDL_MOUSEBUTTONDOWN && e.button == SDL_BUTTON_LEFT) {
		if (m_isWaitingForVelocity) {
			m_masses[m_masses.size() - 1].velocity.x = static_cast<float>(e.x) * renderer.scale() - m_masses[m_masses.size() - 1].position.x;
			m_masses[m_masses.size() - 1].velocity.y = (renderer.height() / renderer.scale() - static_cast<float>(e.y)) * renderer.scale() - m_masses[m_masses.size() - 1].position.y;
			m_masses[m_masses.size() - 1].doIgnore = false;

			m_isWaitingForVelocity = false;
		}
		else {
			Mass newMass;
			newMass.position.x = static_cast<float>(e.x) * renderer.scale();
			newMass.position.y = (renderer.height() / renderer.scale() - static_cast<float>(e.y)) * renderer.scale();
			newMass.velocity.x = 0;
			newMass.velocity.y = 0;
			newMass.acceleration.x = 0;
			newMass.acceleration.y = 0;
			newMass.mass = m_newMassMass;

			newMass.color = m_nextMassColor[0];
			setNextMassColor();

			if (m_doCircularOrbit) {
				newMass.doIgnore = false;
				m_isWaitingForVelocity = false;

				newMass.acceleration = getAcceleration(newMass.position.x, newMass.position.y, -1);

				float angle = std::atan2f(newMass.acceleration.y, newMass.acceleration.x) - M_PI / 2;
				float velocity = std::sqrtf(getLength(newMass.acceleration) * getLength(newMass.position - m_masses[0].position));

				newMass.velocity.x = velocity * std::cosf(angle);
				newMass.velocity.y = velocity * std::sinf(angle);
			}
			else if (e.clicks == 2) {
				newMass.doIgnore = false;
				m_isWaitingForVelocity = false;
			}
			else {
				newMass.doIgnore = true;
				m_isWaitingForVelocity = true;
			}

			m_masses.push_back(newMass);
		}
	}
}

Vector2 GravitySimulator::getAcceleration(float x, float y, int ignoreIndex)
{
	const float G = 66.7408; // [km^3 Yg^-1 s^-1]

	Vector2 result = { 0, 0 };
	Vector2 r;

	for (int i = 0; i < m_masses.size(); i++) {
		if (i != ignoreIndex && !m_masses[i].doIgnore) {
			r.x = m_masses[i].position.x - x;
			r.y = m_masses[i].position.y - y;
			result += r * (G * m_masses[i].mass / (getLength(r) * getLength(r)));
		}
	}

	return result;
}

void GravitySimulator::drawMasses(Renderer& renderer)
{
	for (Mass& mass : m_masses) {
		drawMass(renderer, mass);
	}
}

void GravitySimulator::setNextMassColor()
{
	if (m_nextMassColor == ORANGE) {
		m_nextMassColor = YELLOW;
	}
	else if (m_nextMassColor == YELLOW) {
		m_nextMassColor = GREEN;
	}
	else if (m_nextMassColor == GREEN) {
		m_nextMassColor = ORANGE;
	}
}

void GravitySimulator::drawImGuiOverlay(Renderer& renderer)
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

	float padding = 10.0f;
	ImVec2 position = { renderer.width() / renderer.scale() - padding, padding };
	ImVec2 pivot = { 1.0f, 0.0f };

	ImGui::SetNextWindowPos(position, ImGuiCond_Always, pivot);

	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::Begin("Overlay", nullptr, flags);

	ImGui::Text("Click to place a new mass.");
	ImGui::Text("Click again to set its velocity.");

	ImGui::End();
}

void GravitySimulator::drawImGuiExistingMasses(Renderer&)
{
	const ImVec4 BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };

	ImGui::Begin("Existing Masses");

	if (ImGui::Button("Clear all masses")) {
		m_masses.clear();
	}

	for (int i = 0; i < m_masses.size(); i++) {
		if (m_masses[i].color.x == ORANGE[0].x && m_masses[i].color.y == ORANGE[0].y && m_masses[i].color.z == ORANGE[0].z) {
			ImGui::PushStyleColor(ImGuiCol_Header, ORANGE[0]);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ORANGE[1]);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ORANGE[2]);
		}
		else if (m_masses[i].color.x == YELLOW[0].x && m_masses[i].color.y == YELLOW[0].y && m_masses[i].color.z == YELLOW[0].z) {
			ImGui::PushStyleColor(ImGuiCol_Header, YELLOW[0]);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, YELLOW[1]);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, YELLOW[2]);
			ImGui::PushStyleColor(ImGuiCol_Text, BLACK);
		}
		else if (m_masses[i].color.x == GREEN[0].x && m_masses[i].color.y == GREEN[0].y && m_masses[i].color.z == GREEN[0].z) {
			ImGui::PushStyleColor(ImGuiCol_Header, GREEN[0]);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, GREEN[1]);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, GREEN[2]);
			ImGui::PushStyleColor(ImGuiCol_Text, BLACK);
		}

		std::stringstream ss;
		ss << "Mass ";
		ss << i + 1;

		if (ImGui::CollapsingHeader(ss.str().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PopStyleColor();

			ImGui::BeginTable("Mass Data", 2, ImGuiTableFlags_Borders);

			ImGui::TableNextColumn();
			ImGui::Text("Position:");
			ImGui::TableNextColumn();
			ImGui::Text("(%.1f, %.1f) km", m_masses[i].position.x, m_masses[i].position.y);

			ImGui::TableNextColumn();
			ImGui::Text("Velocity:");
			ImGui::TableNextColumn();
			ImGui::Text("(%.1f, %.1f) km/s", m_masses[i].velocity.x, m_masses[i].velocity.y);

			ImGui::TableNextColumn();
			ImGui::Text("Acceleration:");
			ImGui::TableNextColumn();
			ImGui::Text("(%.1f, %.1f) km/s^2", m_masses[i].acceleration.x, m_masses[i].acceleration.y);

			ImGui::TableNextColumn();
			ImGui::Text("Mass:");
			ImGui::TableNextColumn();
			ImGui::Text("%.1f Yg", m_masses[i].mass);

			ImGui::EndTable();
		}
		else {
			ImGui::PopStyleColor();
		}
	}

	ImGui::End();
}

void GravitySimulator::drawImGuiNewMasses(Renderer&)
{
	if (m_masses.size() == 0)
		m_doCircularOrbit = false;

	ImGui::Begin("New Masses");

	ImGui::InputFloat("Mass [Yg]", &m_newMassMass, 0.0f, 0.0f, "%.1f");
	ImGui::Checkbox("Attempt circular orbit?", &m_doCircularOrbit);

	ImGui::End();
}
