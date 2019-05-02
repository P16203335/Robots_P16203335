#pragma once
#include <SFML/Graphics.hpp>

class Map
{
public:
	Map();
	~Map();
	void update();
	void getWindow(sf::RenderWindow* wind);
private:
	sf::RenderWindow* window;
	std::vector<sf::Shape> shapes;
};