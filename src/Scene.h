//
// Created by Michael Malura on 16/02/16.
//

#ifndef MATCH_TEN_SCENE_H
#define MATCH_TEN_SCENE_H

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Scene {
private:
public:
	virtual void create();

	virtual void destroy();

	virtual void update(sf::Time time);

	virtual void handleEvent(sf::Event &event);

	virtual void render(sf::RenderTarget &target);
};


#endif //MATCH_TEN_SCENE_H
