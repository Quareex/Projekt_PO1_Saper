#include "Rectangle.h"

Rectangle::Rectangle(int x, int y, int width, int height, float rotation, ALLEGRO_COLOR border_color, ALLEGRO_COLOR fill_color) 
	: x(x), y(y), width(width), height(height), rotation(rotation), border_color(border_color), fill_color(fill_color), renderer() {}


void Rectangle::initialize() {
	points.push_back(Point2D(x, y));
	points.push_back(Point2D(x + width, y));
	points.push_back(Point2D(x + width, y + height));
	points.push_back(Point2D(x, y + height));
}

void Rectangle::draw() {
	renderer.drawPolygon(points, border_color);
}

void Rectangle::translate(float dx, float dy) {
	for (auto& point : points) {
		int x = point.getX();
		int y = point.getY();
		point.setX(x + dx);
		point.setY(y + dy);
	}
}

void Rectangle::rotate(float angle) {
	float centerX = 0.0f, centerY = 0.0f;
	for (const auto& point : points) {
		centerX += point.getX();
		centerY += point.getY();
	}
	centerX /= points.size();
	centerY /= points.size();

	for (auto& point : points) { // Use 'auto&' to modify the points
		float x = point.getX();
		float y = point.getY();

		// Translate the point to the origin (relative to the center)
		float translatedX = x - centerX;
		float translatedY = y - centerY;

		// Apply rotation
		float rotatedX = translatedX * cos(angle) - translatedY * sin(angle);
		float rotatedY = translatedX * sin(angle) + translatedY * cos(angle);

		// Translate the point back to the original position
		point.setX(rotatedX + centerX);
		point.setY(rotatedY + centerY);
	}
	
}

void Rectangle::scale(float factor) {
	height *= factor;
	width *= factor;
}