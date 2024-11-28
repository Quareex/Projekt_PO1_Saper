#pragma once
#include <vector>
#include <memory>
#include <allegro5/allegro.h>
#include "DrawableObject.h"
#include "TransformableObject.h"

class BitmapObject : public DrawableObject, public TransformableObject {
protected:
    std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bitmaps;

public:
    // Default constructor
    BitmapObject();

    // Add a bitmap to the object
    void addBitmap(const std::shared_ptr<ALLEGRO_BITMAP>& bitmap);

    // Set a list of bitmaps for the object
    void setBitmaps(const std::vector<std::shared_ptr<ALLEGRO_BITMAP>>& newBitmaps);

    // Draw the object (overrides the draw method from DrawableObject)
    void draw() override;
};