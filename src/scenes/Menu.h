//
// Created by Michael Malura on 16/02/16.
//

#ifndef MATCH_TEN_MENU_H
#define MATCH_TEN_MENU_H

// TinyXML
#include <tinyxml2.h>

// Game
#include "../Scene.h"

class Menu : public Scene {
public:

	virtual void create() override;

	virtual void destroy() override;

	virtual void update(sf::Time time) override;

	virtual void handleEvent(sf::Event &event) override;

	virtual void render(sf::RenderTarget &target) override;
};

#endif //MATCH_TEN_MENU_H
