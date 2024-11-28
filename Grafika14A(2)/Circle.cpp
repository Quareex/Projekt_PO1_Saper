#include "Circle.h"

Circle::Circle(int x, int y, int radius, float rotation, ALLEGRO_COLOR border_color, ALLEGRO_COLOR fill_color)
	: x(x), y(y), radius(radius), rotation(rotation), border_color(border_color), fill_color(fill_color) {}

Circle::Circle(int x, int y, int radius, float rotation, ALLEGRO_COLOR border_color)
	: x(x), y(y), radius(radius), rotation(rotation), border_color(border_color), fill_color(std::nullopt) {}

void Circle::initialize() {

}

void Circle::draw() {
	renderer.drawCircleSymmetric(x, y, radius, border_color);
	if (fill_color.has_value()) {
		renderer.floodFill(x + 1, y + 1, border_color, fill_color.value(), al_get_target_bitmap());
	}
	
}

void Circle::translate(float dx, float dy) {
	x += dx;
	y += dy;
}

void Circle::rotate(float angle) {
	rotation += angle;
}

void Circle::scale(float factor) {
	radius *= factor;
}

