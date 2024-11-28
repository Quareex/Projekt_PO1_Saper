#include "BitmapObject.h"


BitmapObject::BitmapObject() = default;

    // Dodanie bitmapy
void BitmapObject::addBitmap(const std::shared_ptr<ALLEGRO_BITMAP>& bitmap) {
    if (bitmap) {
        bitmaps.push_back(bitmap);
    }
}

    // Ustawienie bitmap
void BitmapObject::setBitmaps(const std::vector<std::shared_ptr<ALLEGRO_BITMAP>>& newBitmaps) {
    bitmaps = newBitmaps;
}

    // Implementacja metody draw()
void BitmapObject::draw() {
    if (bitmaps.empty()) {
        return;
    }

    for (const auto& bitmap : bitmaps) {
        if (bitmap) {
            ALLEGRO_TRANSFORM transform;
            al_identity_transform(&transform);


            al_translate_transform(&transform, -al_get_bitmap_width(bitmap.get()) / 2.0f, -al_get_bitmap_height(bitmap.get()) / 2.0f); // Centrum bitmapy
            al_rotate_transform(&transform, rotation);
            al_scale_transform(&transform, scaleX, scaleY);
            al_translate_transform(&transform, x, y);

            al_use_transform(&transform);
            al_draw_bitmap(bitmap.get(), 0, 0, 0);
        }
    }


}
