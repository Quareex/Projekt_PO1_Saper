#pragma once
#include "ShapeObject.h"
#include "PrimitiveRenderer.h"
#include "Point2D.h"

class Rectangle : public ShapeObject {
private:
	int x, y;
	int width, height;
	float rotation;
	std::vector<Point2D> points;
	ALLEGRO_COLOR border_color;
	ALLEGRO_COLOR fill_color;

	PrimitiveRenderer renderer;

public:
	Rectangle(int x, int y, int width, int height, float rotation, ALLEGRO_COLOR border_color, ALLEGRO_COLOR fill_color);
	~Rectangle() = default;

	void initialize() override;

	void draw() override;

	void translate(float dx, float dy) override;

	void rotate(float angle) override;

	void scale(float factor) override;
};

