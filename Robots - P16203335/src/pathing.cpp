#pragma once
#include <stdlib.h>
#include <Aria.h>
#include <math.h>
#include <iostream>

#include "pathing.h"

pathing::pathing() : ArAction("Pathing") {}

ArActionDesired * pathing::fire(ArActionDesired d)
{
	desiredState.reset();

	return &desiredState;
}