//
// Created by Michael Malura on 16/02/16.
//

#include "Application.h"

Application::Application() : window(sf::VideoMode(768, 768), "Match 10") {
	window.setFramerateLimit(30);
}

void Application::switchScene(Scene *scene) {
	if (currentScene != nullptr) {
		currentScene->destroy();
		delete currentScene;
	}
	currentScene = scene;
}

int Application::run() {
	sf::Time time;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			currentScene->handleEvent(event);
		}
		currentScene->update(time);
		window.clear(sf::Color::Black);
		currentScene->render(window);
		window.display();
	}
	return 0;
}