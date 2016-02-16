//
// Created by Michael Malura on 16/02/16.
//

#include "Game.h"

Game::Game() {
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

void Game::handleEvent(sf::Event &event) {
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

void Game::render(sf::RenderTarget &target) {
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

void Game::update(sf::Time time) {
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