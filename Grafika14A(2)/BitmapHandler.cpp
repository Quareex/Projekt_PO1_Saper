#include "BitmapHandler.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <string>
#include <stdexcept>


    // Konstruktor domyślny
    BitmapHandler::BitmapHandler() : bitmap(nullptr) {}

    // Konstruktor tworzący bitmapę o podanych wymiarach
    BitmapHandler::BitmapHandler(int width, int height) {
        bitmap = al_create_bitmap(width, height);
        if (!bitmap) {
            throw std::runtime_error("Failed to create bitmap.");
        }
    }

    // Konstruktor wczytujący bitmapę z pliku
    BitmapHandler::BitmapHandler(const std::string& filename) {
        bitmap = al_load_bitmap(filename.c_str());
        if (!bitmap) {
            throw std::runtime_error("Failed to load bitmap from file: " + filename);
        }
    }

    // Destruktor
    BitmapHandler::~BitmapHandler() {
        destroy();
    }

    // Usuwanie bitmapy
    void BitmapHandler::destroy() {
        if (bitmap) {
            al_destroy_bitmap(bitmap);
            bitmap = nullptr;
        }
    }

    // Funkcja tworząca nową bitmapę
    void BitmapHandler::create(int width, int height) {
        destroy();
        bitmap = al_create_bitmap(width, height);
        if (!bitmap) {
            throw std::runtime_error("Failed to create bitmap.");
        }
    }

    // Funkcja wczytująca bitmapę z pliku
    void BitmapHandler::loadFromFile(const std::string& filename) {
        destroy();
        bitmap = al_load_bitmap(filename.c_str());
        if (!bitmap) {
            throw std::runtime_error("Failed to load bitmap from file: " + filename);
        }
    }

    // Funkcja zapisująca bitmapę do pliku
    void BitmapHandler::saveToFile(const std::string& filename) const {
        if (!bitmap) {
            throw std::runtime_error("No bitmap to save.");
        }
        if (!al_save_bitmap(filename.c_str(), bitmap)) {
            throw std::runtime_error("Failed to save bitmap to file: " + filename);
        }
    }

    // Funkcja kopiująca bitmapę do innej
    void BitmapHandler::copyTo(BitmapHandler& target) const {
        if (!bitmap) {
            throw std::runtime_error("No source bitmap to copy from.");
        }
        int width = al_get_bitmap_width(bitmap);
        int height = al_get_bitmap_height(bitmap);
        target.create(width, height);

        ALLEGRO_BITMAP* prevTarget = al_get_target_bitmap();
        al_set_target_bitmap(target.bitmap);
        al_draw_bitmap(bitmap, 0, 0, 0);
        al_set_target_bitmap(prevTarget);
    }

    // Funkcja zwracająca wskaźnik do bitmapy
    ALLEGRO_BITMAP* BitmapHandler::getBitmap() const {
        return bitmap;
    }

    // Funkcja ustawiająca wskaźnik do istniejącej bitmapy
    void BitmapHandler::setBitmap(ALLEGRO_BITMAP* newBitmap) {
        destroy();
        bitmap = newBitmap;
    }

    // Funkcja czyszcząca bitmapę
    void BitmapHandler::clearToColor(ALLEGRO_COLOR color) {
        if (!bitmap) {
            throw std::runtime_error("No bitmap to clear.");
        }
        ALLEGRO_BITMAP* prevTarget = al_get_target_bitmap();
        al_set_target_bitmap(bitmap);
        al_clear_to_color(color);
        al_set_target_bitmap(prevTarget);
    }