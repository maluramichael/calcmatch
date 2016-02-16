//
// Created by Michael Malura on 16/02/16.
//

#ifndef MATCH_TEN_APPLICATION_H
#define MATCH_TEN_APPLICATION_H

// SFML
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

// Game
#include "Scene.h"

class Application {
private:
	Scene *currentScene = nullptr;
	sf::RenderWindow window;

public:
	Application();

	void switchScene(Scene *scene);

	int run();
};

#endif //MATCH_TEN_APPLICATION_H
