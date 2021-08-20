
#include <cmath>

#include "Renderer.h"
#include "Vector2.h"

Vector2& operator+=(Vector2& lhs, Vector2 rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

Vector2& operator-=(Vector2& lhs, Vector2 rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

Vector2& operator*=(Vector2& lhs, float rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

Vector2 operator+(Vector2 lhs, Vector2 rhs)
{
	Vector2 result = lhs;
	result += rhs;
	return result;
}

Vector2 operator-(Vector2 lhs, Vector2 rhs)
{
	Vector2 result = lhs;
	result -= rhs;
	return result;
}

Vector2 operator*(Vector2 lhs, float rhs)
{
	Vector2 result = lhs;
	result *= rhs;
	return result;
}

Vector2 operator*(float lhs, Vector2 rhs)
{
	return rhs * lhs;
}

Vector2 operator+(Vector2 vector)
{
	return vector;
}

Vector2 operator-(Vector2 vector)
{
	return vector * -1;
}

float getDotProduct(Vector2 vector1, Vector2 vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y;
}

float getLength(Vector2 vector)
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

Vector2 normalize(Vector2 vector)
{
	return vector * (1 / getLength(vector));
}

void drawVector(Renderer& renderer, float x, float y, Vector2 vector)
{
	const float TIP_SIZE = 10;
	const float TIP_ANGLE = M_PI / 8;

	renderer.drawLine(x, y, x + vector.x, y + vector.y);

	float angle = std::atan2f(-vector.y, -vector.x);

	renderer.drawLine(x + vector.x, y + vector.y, x + vector.x + TIP_SIZE * std::cosf(angle + TIP_ANGLE), y + vector.y + TIP_SIZE * std::sinf(angle + TIP_ANGLE));
	renderer.drawLine(x + vector.x, y + vector.y, x + vector.x + TIP_SIZE * std::cosf(angle - TIP_ANGLE), y + vector.y + TIP_SIZE * std::sinf(angle - TIP_ANGLE));
}
