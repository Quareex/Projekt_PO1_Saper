#pragma once
#include <allegro5/allegro.h>
#include <string>

class BitmapHandler {
private:
	ALLEGRO_BITMAP* bitmap;

public:
	BitmapHandler();
	BitmapHandler(int width, int height);
	BitmapHandler(const std::string& filename);
	~BitmapHandler();

	void destroy();
	void create(int width, int heigth);
	void loadFromFile(const std::string& filename);
	void saveToFile(const std::string& filename) const;
	void copyTo(BitmapHandler& targete) const;
	ALLEGRO_BITMAP* getBitmap() const;
	void setBitmap(ALLEGRO_BITMAP* bitmap);
	void clearToColor(ALLEGRO_COLOR color = al_map_rgb(0, 0, 0));






};

