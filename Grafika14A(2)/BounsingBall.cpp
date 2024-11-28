#include "BouncingBall.h"
#include <allegro5/allegro_primitives.h>
#include <iostream>

// Konstruktor
BouncingBall::BouncingBall(float startX, float startY, float startDx, float startDy, float r, ALLEGRO_COLOR c)
    : circle(startX, startY, r, 0, c), x(startX), y(startY), dx(startDx), dy(startDy), radius(r), color(c) {}

void BouncingBall::initialize() {}

// Implementacja metody animate()
void BouncingBall::animate() {
   
    x += dx;
    y += dy;
    circle.translate(dx, dy);

 
    if (x - radius < 0 || x + radius > 800) { 
        dx = -dx;
    }
    if (y - radius < 0 || y + radius > 600) { 
        dy = -dy;
    }
}


void BouncingBall::draw() {
    circle.draw();
}
