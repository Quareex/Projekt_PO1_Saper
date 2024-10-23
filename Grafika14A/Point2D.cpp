#include "Point2D.h"

Point2D::Point2D() : x(0), y(0) {}

Point2D::Point2D(float x, float y) : x(x), y(y) {}

float Point2D::getX() const {
    return x;
}

float Point2D::getY() const {
    return y;
}

void Point2D::setX(float x) {
    this->x = x;
}

void Point2D::setY(float y) {
    this->y = y;
}

// Rysowanie punktu przy użyciu PrimitiveRenderer
void Point2D::draw(PrimitiveRenderer& renderer, ALLEGRO_COLOR color) const {
    renderer.drawPoint(x, y, color);
}