#pragma once
#ifndef BOUNCINGBALL_H
#define BOUNCINGBALL_H

#include "AnimatedObject.h"
#include "Circle.h"

class BouncingBall : public DrawableObject {
private:
    Circle circle;
    float x, y; 
    float dx, dy; 
    float radius; 
    ALLEGRO_COLOR color = al_map_rgb(255, 255, 255);

public:
    // Konstruktor
    BouncingBall(float startX, float startY, float startDx, float startDy, float r, ALLEGRO_COLOR c);
    void initialize();
    void animate();
    // Implementacja metody draw()
    void draw() override;
};

#endif // BOUNCINGBALL_H
