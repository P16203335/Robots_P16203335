#pragma once

#include <deque>
#include <iostream>
#include <string>
#include <list>
#include <ctime>
#include "sonar.h"
enum dirs
{
	UP,
	LEFT,
	RIGHT,
	UP_RIGHT,
	UP_LEFT,
	OTHER
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
	int threshold = 300;
	int count;
	std::vector<Sonar> sonars;
	int prioritise(std::vector<Sonar> sonars);

	bool first = true;
	clock_t upLeftCD, upRightCD, leftCD, rightCD;

	void adjust(int speed, int deltaHead);
};