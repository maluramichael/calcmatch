//
// Created by Michael Malura on 16/02/16.
//

#ifndef MATCH_TEN_GAME_H
#define MATCH_TEN_GAME_H

// Std
#include <random>

// SFML
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

// Game
#include "../Scene.h"

class Game : public Scene {
private:
	int score = 0;
	int lastScore = 0;
	int gridOffset = 64;

	sf::Clock clock;
	float gameTime = 60;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;

	bool selection = false;
	bool calculate = false;
	std::vector<sf::Vector2i> selectedCells;
	sf::Vector2i currentCell;

	sf::Texture textureCell;
	sf::Sprite spriteCell;
	sf::Texture textureSelection;
	sf::Sprite spriteSelection;
	sf::Text text;

	sf::SoundBuffer soundBufferMatch;
	sf::Sound soundMatch;
	sf::SoundBuffer soundBufferSelection;
	sf::Sound soundSelection;
	sf::Font font;

	short grid[10][10];
	int timeLeft = 0;

public:
	Game();

	void handleEvent(sf::Event &event);

	void render(sf::RenderTarget &target);

	void update(sf::Time time);
};


#endif //MATCH_TEN_GAME_H
