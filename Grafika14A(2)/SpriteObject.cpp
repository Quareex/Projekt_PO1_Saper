#include "SpriteObject.h"
#include <allegro5/allegro.h>

// Constructor to initialize position, bitmaps, and animation properties
SpriteObject::SpriteObject(int x, int y, const std::vector<std::shared_ptr<ALLEGRO_BITMAP>>& bitmaps, int totalFrames, float animationSpeed)
    : BitmapObject(), AnimatedObject(totalFrames, animationSpeed), x(x), y(y) {
    setBitmaps(bitmaps);
}

// Override the animate method to update the current frame based on animation speed
void SpriteObject::animate() {
    AnimatedObject::animate();
}

// Override the draw method to draw the current frame of the sprite
void SpriteObject::draw() {
    if (bitmaps.empty()) return;

    // Draw the current frame of the animation
    al_draw_bitmap(bitmaps[currentFrame].get(), x, y, 0);
}
