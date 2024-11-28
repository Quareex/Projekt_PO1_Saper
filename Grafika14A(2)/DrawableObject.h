#pragma once
#include "GameObject.h"

class DrawableObject : virtual public GameObject {
public:
    virtual void draw() = 0; // Metoda do rysowania obiektu
};

