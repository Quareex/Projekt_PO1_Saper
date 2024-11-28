#pragma once
#include "ShapeObject.h"
#include "PrimitiveRenderer.h"
#include <optional>

class Circle : public ShapeObject {
private:
	int x, y;
	int radius;
	float rotation;
	ALLEGRO_COLOR border_color;
	std::optional<ALLEGRO_COLOR> fill_color;

	PrimitiveRenderer renderer;

public:
	Circle(int x, int y, int radius, float rotation, ALLEGRO_COLOR border_color, ALLEGRO_COLOR fill_color);
	Circle(int x, int y, int radius, float rotation, ALLEGRO_COLOR border_color);
	virtual ~Circle() = default;

	void initialize() override;

	void draw() override;

	void translate(float dx, float dy) override;

	void rotate(float angle) override;

	void scale(float factor) override;
};