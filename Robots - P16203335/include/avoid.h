#pragma once

#include <deque>
#include <iostream>
#include <string>
#include <list>

#include "sonar.h"
enum dirs
{
	UP,
	RIGHT,
	LEFT,
	UP_RIGHT,
	UP_LEFT,
	UP_FAR,
	REVERSE
};

class avoid : public ArAction
{
public:
	avoid();
	virtual ~avoid() {}  // Destructor
	virtual ArActionDesired * fire(ArActionDesired d); // Body of the action
	ArActionDesired desiredState; // Holds state of the robot that we wish to action
private:
	int baseSpeed = 200, speed = 150; // Speed in mm/s
	int DIRECTION;
	int threshold = 400;
	int PAST_DIR;
	int count;
	std::vector<Sonar> sonars;
	int prioritise(std::vector<Sonar> sonars);
};