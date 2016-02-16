#include <iostream>
#include <chrono>
#include <random>
#include <vector>

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define DOUT(x) std::cout << "DEBUG: " << x << '\n';

using namespace std;

class Scene {
private:
public:
	virtual void create() {

	}

	virtual void destroy() {

	}

	virtual void update(sf::Time time) {

	}

	virtual void handleEvent(sf::Event &event) {

	}

	virtual void render(sf::RenderTarget &target) {

	}
};

class Menu : public Scene {

};

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
	Game() {
		DOUT("Create game scene");
		generator = std::default_random_engine((unsigned int) std::time(nullptr));
		distribution = std::uniform_int_distribution<int>(0, 9);

		if (!soundBufferMatch.loadFromFile("assets/powerUp7.ogg")) {
		}

		soundMatch.setBuffer(soundBufferMatch);
		if (!soundBufferSelection.loadFromFile("assets/click1.ogg")) {
		}
		soundSelection.setBuffer(soundBufferSelection);

		if (!font.loadFromFile("assets/AnkaCoder-b.ttf")) {
		}
		text.setFont(font);
		text.setString("1");

		textureCell.loadFromFile("./assets/Red.png");
		textureSelection.loadFromFile("./assets/Selection.png");
		spriteCell.setTexture(textureCell);
		spriteSelection.setTexture(textureSelection);

		for (int y = 0; y < 10; ++y) {
			for (int x = 0; x < 10; ++x) {
				grid[y][x] = (short) distribution(generator);
			}
		}
	}

	void handleEvent(sf::Event &event) {
		switch (event.type) {
			case sf::Event::Closed:

				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
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

	void render(sf::RenderTarget &target) {
		for (int y = 0; y < 10; ++y) {
			for (int x = 0; x < 10; ++x) {
				spriteCell.setPosition(x * 64 + gridOffset, y * 64 + gridOffset);
				text.setPosition(x * 64 + gridOffset + 20, y * 64 + gridOffset + 13);
				text.setString(std::to_string(grid[y][x]));

				target.draw(spriteCell);
				target.draw(text);

				if (currentCell.x == x &&
					currentCell.y == y) {
					spriteSelection.setPosition(x * 64 + gridOffset, y * 64 + gridOffset);
					target.draw(spriteSelection);
				}
			}
		}

		for (int i = 0; i < selectedCells.size(); ++i) {
			spriteSelection.setPosition(selectedCells[i].x * 64 + gridOffset, selectedCells[i].y * 64 + gridOffset);
			target.draw(spriteSelection);
		}

		text.setPosition(16, 16);
		text.setString("Score: " + std::to_string(score) + "/" + std::to_string(lastScore));
		target.draw(text);

		text.setPosition(16, 715);
		text.setString("Time: " + std::to_string(timeLeft));
		target.draw(text);
	}

	void update(sf::Time time) {
		if (currentCell.x < 0) currentCell.x = 0;
		if (currentCell.y < 0) currentCell.y = 0;
		if (currentCell.x >= 10) currentCell.x = 9;
		if (currentCell.y >= 10) currentCell.y = 9;

		timeLeft = (int) ceilf(gameTime - clock.getElapsedTime().asSeconds());

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
	}
};

class Application {
private:
	Scene *currentScene = nullptr;
	sf::RenderWindow window;

public:
	Application() : window(sf::VideoMode(768, 768), "Match 10") {
		DOUT("Create application");
		window.setFramerateLimit(30);
	}

	void switchScene(Scene *scene) {
		DOUT("Switch scene");
		if (currentScene != nullptr) {
			DOUT("Destroy current scene");
			currentScene->destroy();
			DOUT("Delete current scene");
			delete currentScene;
		}
		DOUT("Set new scene");
		currentScene = scene;
	}

	int run() {
		DOUT("Run application");
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
};

int main() {
	DOUT("Start main");
	Application application;
	application.switchScene(new Game());
	return application.run();
}

