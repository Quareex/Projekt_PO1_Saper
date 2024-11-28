#ifndef POINT2D_H
#define POINT2D_H

#include "PrimitiveRenderer.h"

class Point2D {
public:
    // Domyślny konstruktor
    Point2D();

    // Konstruktor ze współrzędnymi
    Point2D(float x, float y);

    float getX() const;

    float getY() const;

    void setX(float x);

    void setY(float y);

    void draw(PrimitiveRenderer& renderer, ALLEGRO_COLOR color) const;

private:
    float x;
    float y;
};

#endif
