#include <SFML/Graphics.hpp>

#include <ctime>
#include "sonar.h"

class mapping : public ArAction
{
public:
	mapping();
	virtual ~mapping() {};
	virtual ArActionDesired * fire(ArActionDesired d); // Body of the action
	ArActionDesired desiredState; // Holds state of the robot that we wish to action
private:
	std::vector<Sonar> sonars;
	std::vector <sf::CircleShape> circles;
	sf::RenderWindow window;
	sf::RectangleShape robot;
};