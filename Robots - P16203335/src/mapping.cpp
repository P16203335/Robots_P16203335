#include <stdlib.h>
#include <Aria.h>
#include <math.h>

#include "mapping.h"

mapping::mapping() : ArAction("Mapping") {}

ArActionDesired * mapping::fire(ArActionDesired d)
{	
	return &desiredState;
}