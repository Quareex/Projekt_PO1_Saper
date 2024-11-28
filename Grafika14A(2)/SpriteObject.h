#include "BitmapObject.h"
#include "AnimatedObject.h"

class SpriteObject : public BitmapObject, public AnimatedObject {
private:
    int x, y; // Position of the sprite

public:
    // Constructor
    SpriteObject(int x, int y, const std::vector<std::shared_ptr<ALLEGRO_BITMAP>>& bitmaps, int totalFrames, float animationSpeed);

    // Override the animate method to update the current frame
    void animate() override;

    void draw() override;
};




