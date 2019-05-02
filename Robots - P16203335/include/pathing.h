#pragma once

class pathing : public ArAction
{
public:
	pathing();
	virtual ~pathing() {};
	virtual ArActionDesired * fire(ArActionDesired d); // Body of the action
	ArActionDesired desiredState; // Holds state of the robot that we wish to action
private:
};