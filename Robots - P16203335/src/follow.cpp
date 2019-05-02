// follow.cpp
#include <stdlib.h>
#include <Aria.h>
#include <math.h>

#include "follow.h"
follow::follow() : ArAction("Follow line") {}

ArActionDesired * follow::fire(ArActionDesired d)
{
	desiredState.reset(); // Reset the desired state (must be done)
	deltaHeading = 0;
	unsigned int cycleTime = myRobot->getCycleTime();

	// Get sonar readings

	//Left sonars
	lTopSonar = myRobot->getSonarRange(1);		//Top
	lTMidSonar = myRobot->getSonarRange(0);		//Top-middle
	lBMidSonar = myRobot->getSonarRange(15);	//Bottom-middle
	lBotSonar = myRobot->getSonarRange(14);		//Bottom

	//Right sonars
	rTopSonar = myRobot->getSonarRange(6);		//Top	
	rTMidSonar = myRobot->getSonarRange(7);		//Top-middle
	rBMidSonar = myRobot->getSonarRange(8);		//Bottom-middle
	rBotSonar = myRobot->getSonarRange(9);		//Bottom

	double closestLeft = min(min(lTopSonar, lTMidSonar), min(lBMidSonar, lBotSonar));	//Returns the closest sonar on the left

	double closestRight = min(min(rTopSonar, rTMidSonar), min(rBMidSonar, rBotSonar));	//Closest on the right

	// Collision avoidance
	//frontSonar = myRobot->getClosestSonarRange(-20, 20);	//Gets the closest sonar at the between -20 and 20 (the front)
	//speed = frontSonar > 750 ? baseSpeed : (unsigned int)(max((frontSonar / 750), 0.25) * baseSpeed);	//If there is something closer than 750mm, then gradually slow to a minimum of 25% baseSpeed
	//double followProp = frontSonar > 750 ? 1.0 : frontSonar / 750;

	bool followingRight = closestLeft > closestRight;	//Check if we're following on the right or left

	double closest = followingRight ? min(min(rTopSonar, rTMidSonar), rBMidSonar) : min(min(lTopSonar, lTMidSonar), lBMidSonar);	//Get the closest sonar on the right or left
	/*std::cout << "Line following:" << std::endl;*/

	if (closest > snapOffset)
	{	//If the closest is further than the line follow limit, stop following
		desiredState.setVel(speed); // set the speed of the robot in the desired state
		desiredState.setDeltaHeading(0); // Set the heading change of the robot

		//std::cout << "Stopped line following." << std::endl << std::endl;
		return &desiredState; // give the desired state to the robot for actioning
	}
	//else
	//{	//If the closest is closer than the limit, continue
	//	std::cout << "Closest sonar: " << closest << ". Following line." << std::endl;
	//}
	double angle;
	bool active;

	// Parallel Beam Component
	double midSonarDelta = followingRight ? rBMidSonar - rTMidSonar : lBMidSonar - lTMidSonar;
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
		double topX = cosForty * rTopSonar;
		double topY = sinForty * rTopSonar;
		double botX = cosForty * rBotSonar;
		double botY = sinForty * rBotSonar;

		double y = topY + botY + 225.0;
		double x = botX - topX;

		angle = ((atan2(y, x) * 180.0 / M_PI) - 90.0) * -1.0;
	}

	else
	{
		double topX = cosForty * lTopSonar;
		double topY = sinForty * lTopSonar;
		double botX = cosForty * lBotSonar;
		double botY = sinForty * lBotSonar;

		double y = topY + botY + 225.0;
		double x = botX - topX;

		angle = (atan2(y, x) * 180.0 / M_PI) - 90.0;
	}

	active = std::abs(angle) < 50.0;
	deltaHeading += m_wideComp.update(cycleTime, active, angle);

	// Offset Component
	double err = max(desiredOffset - closest, -desiredOffset);
	angle = err * 0.002 * 50;
	if (!followingRight)
	{ 
		angle *= -1.0;
	}

	deltaHeading += m_offsetComp.update(cycleTime, true, angle);
	/*
	double turningSpeedFactor = (10.0 - (min(std::abs(deltaHeading), 10.0) * 0.5)) * 0.1;

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
	*/
	//std::cout << "Follow Proportion: " << followProp << std::endl;
	//std::cout << "Turn Speed Factor: " << turningSpeedFactor << std::endl;

	/*std::cout << "Delta Heading: " << deltaHeading << std::endl;
	std::cout << "Speed: " << speed << std::endl << std::endl;*/	

	desiredState.setVel(speed); // set the speed of the robot in the desired state
	desiredState.setDeltaHeading(deltaHeading); // Set the heading change of the robot

	return &desiredState; // give the desired state to the robot for actioning
}