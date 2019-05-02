#pragma once
#include <ctime>
class wander : public ArAction
{
public:
	wander();
	virtual ~wander() {};
	virtual ArActionDesired * fire(ArActionDesired d); // Body of the action
	ArActionDesired desiredState; // Holds state of the robot that we wish to action
private:
	std::clock_t start = 0;
	double duration;
	bool first = true;
	int timeStep;
	int baseSpeed = 200;
};