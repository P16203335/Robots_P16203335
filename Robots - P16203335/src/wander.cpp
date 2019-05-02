#pragma once
#include <stdlib.h>
#include <Aria.h>
#include <iostream>
#include "wander.h"

wander::wander() : ArAction("Wandering") {}

ArActionDesired * wander::fire(ArActionDesired d)
{
	desiredState.reset();
	desiredState.setVel(baseSpeed);
	if (first)
	{
		start = clock();
		timeStep = 3 + rand() % 5 + 1;
		first = false;
	}
	if (((clock() - start) / (double)CLOCKS_PER_SEC) > timeStep)
	{
		double deltaHead = rand() % 100 - 50;
		std::cout << "Wandering:"<<deltaHead<<"\n";
		myRobot->setDeltaHeading(deltaHead);
		start = clock();
		timeStep = 3 + rand() % 5 + 1;
	}
	return &desiredState;
}