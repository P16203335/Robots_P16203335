// follow.cpp
#include <Aria.h>
#include <math.h>

#include "follow.h"
#include "sonar.h"
follow::follow() : ArAction("Follow line") {}
 
ArActionDesired * follow::fire(ArActionDesired d)
{
	desiredState.reset(); // Reset the desired state (must be done)
	deltaHeading = 0;
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
	frontSonar = myRobot->getClosestSonarRange(-20, 20);	//Gets the closest sonar between -20 and 20 (the front)
	double followProp = frontSonar > 750 ? 1.0 : frontSonar / 750;

	bool followingRight = closestLeft > closestRight;	//Check if we're following on the right or left

	double closest = followingRight ? smaller(smaller(rightSonars[0].range, rightSonars[1].range), rightSonars[2].range) : smaller(smaller(leftSonars[0].range, leftSonars[1].range), leftSonars[2].range);

	if (closest > snapOffset)
	{	//If the closest is further than the line follow limit, stop following
		desiredState.setVel(speed); // set the speed of the robot in the desired state
		desiredState.setDeltaHeading(0); // Set the heading change of the robot

		return &desiredState; // give the desired state to the robot for actioning
	}
	double angle;
	bool active;

	// Parallel Beam Component
	double midSonarDelta = followingRight ? rightSonars[2].range - rightSonars[1].range : leftSonars[2].range - leftSonars[1].range;
	angle = (atan2(225.0, midSonarDelta) * 180.0 / M_PI) - 90.0;
	if (followingRight)
	{ 
		angle *= -1.0;
	}
	
	active = std::abs(angle) < 50.0;
	deltaHeading += m_parallelComp.update(cycleTime, active, angle);

	double cosForty = cos(40 * M_PI / 180); // Cosine of forty degrees.
	double sinForty = sin(40 * M_PI / 180); // Sine of forty degrees.

	// Wide Beam Component
	if (followingRight)
	{
		double topX = cosForty * rightSonars[0].range;
		double topY = sinForty * rightSonars[0].range;
		double botX = cosForty * rightSonars[3].range;
		double botY = sinForty * rightSonars[3].range;
		
		angle = ((atan2((topY + botY + 225.0), (botX - topX)) * 180.0 / M_PI) - 90.0) * -1.0;
	}
	else
	{
		double topX = cosForty * leftSonars[0].range;
		double topY = sinForty * leftSonars[0].range;
		double botX = cosForty * leftSonars[3].range;
		double botY = sinForty * leftSonars[3].range;
		
		angle = (atan2((topY + botY + 225.0), (botX - topX)) * 180.0 / M_PI) - 90.0;
	}

	active = std::abs(angle) < 50.0;
	deltaHeading += m_wideComp.update(cycleTime, active, angle);

	// Offset Component
	double err = bigger(desiredOffset - closest, -desiredOffset);
	angle = err * 0.002 * 50;
	if (!followingRight)
	{ 
		angle *= -1.0;
	}

	deltaHeading += m_offsetComp.update(cycleTime, true, angle);
	
	double turningSpeedFactor = (10.0 - (smaller(std::abs(deltaHeading), 10.0) * 0.5)) * 0.1;

	double avoidHeading = (1 - followProp) * 50;
	if (!followingRight) 
	{
		avoidHeading *= -1.0;
	}

	deltaHeading = avoidHeading + (followProp * deltaHeading);

	if (followProp > turningSpeedFactor) 
	{ 
		speed = (unsigned int)(baseSpeed * turningSpeedFactor);
	}
	
	desiredState.setVel(speed); // set the speed of the robot in the desired state
	desiredState.setDeltaHeading(deltaHeading); // Set the heading change of the robot

	return &desiredState; // give the desired state to the robot for actioning
}