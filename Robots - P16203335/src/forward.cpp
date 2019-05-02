#include <iostream>
#include <stdlib.h>
#include <Aria.h>

#include "forward.h"
// Implementation

// Constructor
forward::forward() : ArAction("Go forward")
{
	speed = 50; // Set the robots speed to 50 mm/s. 200 is top speed
}

// Body of action
ArActionDesired * forward::fire(ArActionDesired d)
{
	desiredState.reset(); // reset the desired state (must be done)
	desiredState.setVel(speed); // set the speed of the robot in the desired state

	printf("%f %f %f\n", myRobot->getX(), myRobot->getY(), myRobot->getTh());
	return &desiredState; // give the desired state to the robot for actioning
}