//
// Created by Michael Malura on 16/02/16.
//

#include <iostream>
#include "Menu.h"

void Menu::create() {
	tinyxml2::XMLDocument doc;
	doc.LoadFile("assets/greySheet.xml");
	tinyxml2::XMLElement *root = doc.FirstChildElement("TextureAtlas");
	tinyxml2::XMLElement *element = root->FirstChildElement("SubTexture");
	do {
		int width = -1, height = -1, x = -1, y = -1;
		element->QueryAttribute("width", &width);
		element->QueryAttribute("height", &height);
		element->QueryAttribute("x", &x);
		element->QueryAttribute("y", &y);

		const char *name = element->Attribute("name");
		std::cout << name << " " << x << " " << y << " " << width << " " << height << '\n';
		element = element->NextSiblingElement("SubTexture");
	} while (element);
}

void Menu::destroy() {
}

void Menu::update(sf::Time time) {
}

void Menu::handleEvent(sf::Event &event) {
}

void Menu::render(sf::RenderTarget &target) {
}
