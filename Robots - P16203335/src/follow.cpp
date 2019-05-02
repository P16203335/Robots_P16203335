// follow.cpp
#include <Aria.h>
#include <math.h>

#include "follow.h"
#include "sonar.h"
follow::follow() : ArAction("Follow line") {}
 
ArActionDesired * follow::fire(ArActionDesired d)
{
	desiredState.reset(); // Reset the desired state (must be done)
	deltaHead = 0;
	unsigned int cycleTime = myRobot->getCycleTime();	//The time in ms between each cycle
	//Sonar array for both left and right
	Sonar leftSonars[4], rightSonars[4];
	
	//Setting the sonar numbers of the left sonars top to bottom
	leftSonars[0].number = 1;		//Top
	leftSonars[1].number = 0;		//Top-middle
	leftSonars[2].number = 15;	//Bottom-middle
	leftSonars[3].number = 14;		//Bottom
		
	for (int i = 0; i < 4; i++)
	{
		//Iterates through the right sonars adding their sonar numbers
		rightSonars[i].number = i + 6;

		//Iterates for both to get their range
		leftSonars[i].range = myRobot->getSonarRange(leftSonars[i].number);		
		rightSonars[i].range = myRobot->getSonarRange(i + 6);
	}
	//Find the closest sonar for both sides by coparing
	double closestLeft = smaller(smaller(leftSonars[0].range, leftSonars[1].range), smaller(leftSonars[2].range, leftSonars[3].range));
	double closestRight = smaller(smaller(rightSonars[0].range, rightSonars[1].range), smaller(rightSonars[2].range, rightSonars[3].range));

	// Collision avoidance
	front = myRobot->getClosestSonarRange(-20, 20);	//Gets the closest sonar between -20 and 20 (the front)
	speed = front > 800 ? baseSpeed : (unsigned int)(bigger((front / 800), 0.25) * baseSpeed);
	double followProp = front > 800 ? 1.0 : front / 800;

	bool followRight = closestLeft > closestRight;	//Check if we're following on the right or left

	double closest = followRight ? smaller(smaller(rightSonars[0].range, rightSonars[1].range), rightSonars[2].range) : smaller(smaller(leftSonars[0].range, leftSonars[1].range), leftSonars[2].range);

	if (closest > snapOffset)
	{	//If the closest is further than the line follow limit, stop following
		desiredState.setVel(speed); // set the speed of the robot in the desired state
		desiredState.setDeltaHeading(0); // Set the heading change of the robot

		return &desiredState; // give the desired state to the robot for actioning
	}
	double ang;
	bool active;

	// Parallel Beam Component
	//Get the difference between the two parallel sensors
	double midSonarDelta = followRight ? rightSonars[2].range - rightSonars[1].range : leftSonars[2].range - leftSonars[1].range;
	ang = (atan2(225.0, midSonarDelta) * 180.0 / M_PI) - 90.0;
	if (followRight)
	{ 
		ang *= -1.0;	//Inverse the ang if we're following right
	}
	
	active = std::abs(ang) < 50.0;	//If the ang is less than 50 set to active
	deltaHead += parallel.update(cycleTime, active, ang);	//Update the deltaHead with the new values

	double cosForty = cos(40 * M_PI / 180); // Cosine of forty degrees.
	double sinForty = sin(40 * M_PI / 180); // Sine of forty degrees.

	// Wide Beam Component
	if (followRight)
	{
		//Mutiply the top and bottom sonar ranges by cos(40) and sin(40) in radians
		double topX = cosForty * rightSonars[0].range;
		double topY = sinForty * rightSonars[0].range;
		double botX = cosForty * rightSonars[3].range;
		double botY = sinForty * rightSonars[3].range;
		//Total for y and get the difference for x
		double y = topY + botY + 225.0;
		double x = botX - topX;
		//Find the ang
		ang = ((atan2(y, x) * 180.0 / M_PI) - 90.0) * -1.0;
	}
	else
	{	//Repeat above but for left hand sonars
		double topX = cosForty * leftSonars[0].range;
		double topY = sinForty * leftSonars[0].range;
		double botX = cosForty * leftSonars[3].range;
		double botY = sinForty * leftSonars[3].range;
		double y = topY + botY + 225.0;
		double x = botX - topX;

		ang = ((atan2(y, x) * 180.0 / M_PI) - 90.0);
	}

	active = std::abs(ang) < 50.0;
	deltaHead += wide.update(cycleTime, active, ang);	//Update deltaHead

	// Offset Component
	//Get the biggest value between the delta of the desired offset and the closest range, and the inverse of the offset
	double i = bigger(goalOffset - closest, -goalOffset);
	//Find the new ang
	ang = i * 0.002 * 50;
	if (!followRight)
	{ 
		ang *= -1.0;
	}

	deltaHead += offset.update(cycleTime, true, ang);	//Update the deltaHead
	
	double turnSpeed = (10.0 - (smaller(std::abs(deltaHead), 10.0) * 0.5)) * 0.1;

	double avoidHead = (1 - followProp) * 50;
	if (!followRight) 
	{
		avoidHead *= -1.0;
	}

	deltaHead = avoidHead + (followProp * deltaHead);

	if (followProp > turnSpeed) 
	{ 
		speed = (unsigned int)(baseSpeed * turnSpeed);
	}
	
	desiredState.setVel(speed); // set the speed of the robot in the desired state
	desiredState.setDeltaHeading(deltaHead); // Set the heading change of the robot

	return &desiredState; // give the desired state to the robot for actioning
}