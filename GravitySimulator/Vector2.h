
#pragma once

#include "Renderer.h"

struct Vector2 {
	float x, y;
};

Vector2& operator+=(Vector2& lhs, Vector2 rhs);
Vector2& operator-=(Vector2& lhs, Vector2 rhs);
Vector2& operator*=(Vector2& lhs, float rhs);

Vector2 operator+(Vector2 lhs, Vector2 rhs);
Vector2 operator-(Vector2 lhs, Vector2 rhs);
Vector2 operator*(Vector2 lhs, float rhs);
Vector2 operator*(float lhs, Vector2 rhs);

Vector2 operator+(Vector2);
Vector2 operator-(Vector2);

float getDotProduct(Vector2, Vector2);
float getLength(Vector2);
Vector2 normalize(Vector2);

void drawVector(Renderer&, float x, float y, Vector2);