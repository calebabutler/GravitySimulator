
#include "Mass.h"

void applyAcceleration(Mass& mass, float secondsPerFrame)
{
	if (!mass.doIgnore) {
		mass.velocity += mass.acceleration * secondsPerFrame; // [km/s] += [km/s^2]*[s]
		mass.position += mass.velocity * secondsPerFrame; // [km] += [km/s]*[s]
	}
}

void drawMass(Renderer& renderer, const Mass& mass)
{
	const float MASS_RADIUS = 5;

	if (!mass.doIgnore) {
		renderer.setColor(1.0f, 0.0f, 0.0f);
		drawVector(renderer, mass.position.x, mass.position.y, mass.acceleration);
		renderer.setColor(0.0f, 0.0f, 1.0f);
		drawVector(renderer, mass.position.x, mass.position.y, mass.velocity);
	}

	renderer.setColor(mass.color.x, mass.color.y, mass.color.z);
	renderer.drawCircle(mass.position.x, mass.position.y, MASS_RADIUS);
}
