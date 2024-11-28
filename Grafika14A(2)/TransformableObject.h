#pragma once
#include "GameObject.h"

class TransformableObject : virtual public GameObject {
protected:
    int x = 0, y = 0;
    float rotation = 0;
    float scaleX = 1, scaleY = 1;
public:
    virtual void translate(float dx, float dy) = 0;
    virtual void rotate(float angle) = 0;
    virtual void scale(float factor) = 0;
};
