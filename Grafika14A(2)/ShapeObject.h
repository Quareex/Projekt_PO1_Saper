#pragma once
#include "DrawableObject.h"
#include "TransformableObject.h"
#include "allegro5/allegro.h"

class ShapeObject : public DrawableObject, public TransformableObject {
public:
	virtual ~ShapeObject() = default;
};

