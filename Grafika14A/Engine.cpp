#include "Engine.h"
#include "PrimitiveRenderer.h"
#include "Point2D.h"
#include "LineSegment.h"

Engine::Engine()
	:display(nullptr), eventQueue(nullptr), timer(nullptr),
	screenWidth(800), screenHeight(600), running(false), fps(60), currentBuffer(), renderer(display) {
	//Inicjalizacja wskaźników do bitmap
	for (int i = 0; i < 3; i++) {
		backBuffer[i] = nullptr;
	}
}

Engine::~Engine(){
	shutdown();
 }

bool Engine::init(int width, int height, bool fullscreen, int fps, bool useTripleBuffer) {
	this->screenWidth = width;
	this->screenHeight = height;
	this->fps = fps;

	if (!al_init()) {
		logError("Nie udalo sie incjalizowacz Allego");
		return false;
	}

	//al_init_image_addon();
	//al_init_font_addon();
	//al_init_ttf_addon();
	//al_init_primitives_addon();
	al_install_mouse();
	al_install_keyboard();
	//al_init_native_dialog_addon();

	screenWidth = width;
	screenHeight = height;
	this->fps = fps;

	if (fullscreen) {
		display = al_create_display(width, height);
	}
	else {
		display = al_create_display(width, height);
	}

	if (!display) {
		logError("Nie udalo sie stworzyc display");
		return false;
	}

	if (useTripleBuffer) {
		for (int i = 0; i < 3; ++i) {
			backBuffer[i] = al_create_bitmap(width, height);
		}
	}
	else {
		backBuffer[0] = al_create_bitmap(width, height);
	}

	timer = al_create_timer(1.0 / fps);
	al_start_timer(timer);

	eventQueue = al_create_event_queue();
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_mouse_event_source());
	al_register_event_source(eventQueue, al_get_keyboard_event_source());
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));

	running = true;
	return true;
}

void Engine::run() {
	std::vector<Point2D> polygonPoints = {
	Point2D(100, 100),
	Point2D(200, 200),
	Point2D(200, 100),
	Point2D(100, 200)
	};
	int circleX = 400;
	int circleY = 300;
	int radius = 100;

	std::vector<Point2D> points = {
		Point2D(150, 150),
		Point2D(300, 200),
		Point2D(450, 250),
		Point2D(600, 100)
	};
	Point2D point1(150, 150);
	Point2D point2(400, 300);
	Point2D point3(555, 350);
	Point2D point4(700, 400);
	LineSegment line(point3, point4);

	while (running) {
		handleEvents();

		al_set_target_bitmap(backBuffer[currentBuffer]);

		clearScreen();

		point1.draw(renderer, al_map_rgb(255, 0, 0));
		point2.draw(renderer, al_map_rgb(0, 255, 0));

		line.draw(renderer, al_map_rgb(255, 255, 0));

		renderer.drawLineIncremental(50, 50, 300, 200, al_map_rgb(255, 0, 0));
		renderer.drawLineIncremental(100, 100, 200, 300, al_map_rgb(0, 255, 0));

		renderer.drawPolyline(points, al_map_rgb(255, 0, 0));

		renderer.drawClosedPolyline(points, al_map_rgb(0, 255, 0));

		renderer.drawCircleSymmetric(circleX, circleY, radius, al_map_rgb(255, 0, 0));
		renderer.drawEllipse(400, 300, 150, 100, al_map_rgb(0, 0, 255));
		renderer.drawPolygon(polygonPoints, al_map_rgb(255, 255, 255));


		al_set_target_backbuffer(display);
		al_draw_bitmap(backBuffer[currentBuffer], 0, 0, 0);
		al_flip_display();

		currentBuffer = (currentBuffer + 1) % 3; // triple buffer
	}
}

void Engine::shutdown() {
	if (display) {
		al_destroy_display(display);
	}
	if (eventQueue) {
		al_destroy_event_queue(eventQueue);
	}
	if (timer) {
		al_destroy_timer(timer);
	}
}

void Engine::handleEvents() {
	ALLEGRO_EVENT event;
	while (al_get_next_event(eventQueue, &event)) {
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			running = false;
		}
		if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			std::cout << "Mouse moved to: " << event.mouse.x << ", " << event.mouse.y << std::endl;
		}
		if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			std::cout << "Key pressed: " << event.keyboard.keycode << std::endl;
		}
	}
}

void Engine::clearScreen() {
	al_clear_to_color(al_map_rgb(0, 0, 0));
}

void Engine::logError(const std::string& message) {
	std::cerr << "Error" << message << std::endl;
}
