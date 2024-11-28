#ifndef ENGINE_H
#define ENGINE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/mouse.h>
#include <iostream>
#include <fstream>
#include "PrimitiveRenderer.h"

class Engine {
public:
	Engine(); //konstrukrot dla naczalnoi inizializacii dvizka
	~Engine(); //Dekonstruktor

	bool init(int width, int height, bool fullscreen = false, int fps = 60, bool useTripleBuffer = false);

	void run();
	void clearScreen();
	void handleEvents();
	//void clearBitmap(ALLEGRO_BITMAP* bitmap, ALLEGRO_COLOR color);
	void logError(const std::string& massage);

private:
	void shutdown();
	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* eventQueue;
	ALLEGRO_TIMER* timer;
	ALLEGRO_BITMAP* backBuffer[3];

	int currentBuffer;
	int screenWidth;
	int screenHeight;
	bool running;
	int fps;
	PrimitiveRenderer renderer;

};

#endif ENGINE_H

