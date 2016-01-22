#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <entityx/entityx.h>

namespace ex = entityx;

struct Sprite {
	Sprite(sf::Texture texture) : sprite(texture) {

	};

	sf::Sprite sprite;
};

struct Position {
	Position(float x = 0.0f, float y = 0.0f) : x(x), y(y) { }

	float x, y;
};

struct Value {
	Value(int value) : value(value) { }

	int value;
};

class Level : public ex::EntityX {
public:
	explicit Level(int difficulty) {
		//systems.add<DebugSystem>();
		//systems.add<KeyboardSystem>();
		//systems.add<CollisionSystem>();
		systems.configure();

		generate();
	}

	void generate() {
		for (auto x = 0; x < 10; x++) {
			entityx::Entity entity = entities.create();
			entity.assign<Position>(rand() % 100, rand() % 100);
			entity.assign<Sprite>(rand() % 100, rand() % 100);
		}
	}

	void update(sf::Time delta) {
		//systems.update<DebugSystem>(delta);
		//systems.update<KeyboardSystem>(delta);
		//systems.update<CollisionSystem>(delta);
	}
};

int main() {

	entityx::EntityX world;

	int score = 0;
	int lastScore = 0;
	int gridOffset = 64;

	sf::Clock clock;
	float gameTime = 60;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 9);

	sf::SoundBuffer soundBufferMatch;
	if (!soundBufferMatch.loadFromFile("assets/powerUp7.ogg")) {
		return 1;
	}

	sf::Sound soundMatch;
	soundMatch.setBuffer(soundBufferMatch);

	sf::SoundBuffer soundBufferSelection;
	if (!soundBufferSelection.loadFromFile("assets/click1.ogg")) {
		return 1;
	}

	sf::Sound soundSelection;
	soundSelection.setBuffer(soundBufferSelection);

	sf::Font font;
	if (!font.loadFromFile("assets/AnkaCoder-b.ttf")) {
		return 1;
	}

	bool selection = false;
	bool calculate = false;
	std::vector<sf::Vector2i> selectedCells;
	sf::Vector2i currentCell;

	sf::Texture textureCell;
	sf::Sprite spriteCell;
	sf::Texture textureSelection;
	sf::Sprite spriteSelection;
	sf::Text text;
	text.setFont(font);
	text.setString("1");

	textureCell.loadFromFile("./assets/Red.png");
	textureSelection.loadFromFile("./assets/Selection.png");

	sf::RenderWindow window(sf::VideoMode(768, 768), "Match 10");
	window.setFramerateLimit(30);
	bool isRunning = true;
	while (isRunning && window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					switch (event.key.code) {
						case sf::Keyboard::Escape:
							isRunning = false;
							break;
						case sf::Keyboard::Left:
							soundSelection.play();
							currentCell.x -= 1;
							break;
						case sf::Keyboard::Right:
							soundSelection.play();
							currentCell.x += 1;
							break;
						case sf::Keyboard::Up:
							soundSelection.play();
							currentCell.y -= 1;
							break;
						case sf::Keyboard::Down:
							soundSelection.play();
							currentCell.y += 1;
							break;
						case sf::Keyboard::Space:
							selection = true;
							break;
					}
					break;
				case sf::Event::KeyReleased:
					switch (event.key.code) {
						case sf::Keyboard::Space:
							selection = false;
							calculate = true;
							break;
					}
					break;
			}
		}

		if (currentCell.x < 0) currentCell.x = 0;
		if (currentCell.y < 0) currentCell.y = 0;
		if (currentCell.x >= 10) currentCell.x = 9;
		if (currentCell.y >= 10) currentCell.y = 9;

		int timeLeft = (int) ceilf(gameTime - clock.getElapsedTime().asSeconds());

		if (timeLeft == 0) {
			lastScore = score;
			score = 0;
			clock.restart();
		}

		if (calculate) {
			int result = 0;
			for (auto cell : selectedCells) {
				result += grid[cell.y][cell.x];
			}

			if (result == 10) {
				score += pow(2, selectedCells.size());

				for (auto cell : selectedCells) {
					grid[cell.y][cell.x] = (short) distribution(generator);
				}
				soundMatch.play();
			}

			calculate = false;
			selectedCells.clear();
		}

		if (selection) {
			auto found = false;
			for (auto cell : selectedCells) {
				if (cell.x == currentCell.x &&
					cell.y == currentCell.y) {
					found = true;
					break;
				}
			}
			if (!found) {
				selectedCells.push_back(sf::Vector2i(currentCell.x, currentCell.y));
			}
		}

		window.clear(sf::Color::Black);

		for (const int y = 0; y < 10; ++y) {
			for (const int x = 0; x < 10; ++x) {
				spriteCell.setPosition(x * 64 + gridOffset, y * 64 + gridOffset);
				text.setPosition(x * 64 + gridOffset + 20, y * 64 + gridOffset + 13);
				text.setString(std::to_string(grid[y][x]));

				window.draw(spriteCell);
				window.draw(text);

				if (currentCell.x == x &&
					currentCell.y == y) {
					spriteSelection.setPosition(x * 64 + gridOffset, y * 64 + gridOffset);
					window.draw(spriteSelection);
				}
			}
		}

		for (int i = 0; i < selectedCells.size(); ++i) {
			spriteSelection.setPosition(selectedCells[i].x * 64 + gridOffset, selectedCells[i].y * 64 + gridOffset);
			window.draw(spriteSelection);
		}

		text.setPosition(16, 16);
		text.setString("Score: " + std::to_string(score) + "/" + std::to_string(lastScore));
		window.draw(text);

		text.setPosition(16, 715);
		text.setString("Time: " + std::to_string(timeLeft));
		window.draw(text);

		window.display();
	}
	window.close();

	return 0;
}

