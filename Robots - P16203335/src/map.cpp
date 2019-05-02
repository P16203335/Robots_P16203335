#include "map.h"

Map::Map(){}
Map::~Map(){}

void Map::update()
{
	window->clear(sf::Color().Black);
	for (int i = 0; i< shapes.size(); i++)
	{
		window->draw(shapes.at(i));
	}
	window->display();
}

void Map::getWindow(sf::RenderWindow* wind)
{
	window = wind;
}