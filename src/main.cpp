// Std
#include <iostream>

// Game
#include "Scene.h"
#include "Scenes/Game.h"
#include "Application.h"

int main() {
	Application application;
	application.switchScene(new Game());
	return application.run();
}

