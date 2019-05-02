#pragma once
#include <stdlib.h>
#include <Aria.h>
#include <math.h>

#include "mapping.h"

mapping::mapping() : ArAction("Mapping") {}

ArActionDesired * mapping::fire(ArActionDesired d)
{	
	desiredState.reset();
	for (int i = 0; i < 16; i++)	//For all sonars
	{
		if (myRobot->getSonarRange(i) < 5000)
		{
			Sonar sonar;
			sonar.range = myRobot->getSonarRange(i);
			sonar.number = i;
			sonars.push_back(sonar);
		}
	}
	for (Sonar sonar : sonars)
	{
		//DO MATHS
		//DRAW A CIRCLE
	}
	return &desiredState;
}