#include "Engine.h"

int main() {
	Engine engine;

	if (!engine.init(800, 600, false, 60, true)) {
		return -1;
	}

	engine.run();

	return 0;
}