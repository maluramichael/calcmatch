// Game
#include "Scene.h"
#include "Scenes/Menu.h"
#include "Application.h"

int main() {
	Application application;
	application.switchScene(new Menu());
	return application.run();
}