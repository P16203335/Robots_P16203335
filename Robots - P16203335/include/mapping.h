#pragma once

class mapping : public ArAction
{
public:
	mapping();
	virtual ~mapping();
	virtual ArActionDesired * fire(ArActionDesired d); // Body of the action
	ArActionDesired desiredState; // Holds state of the robot that we wish to action
private:
};