#include <stdlib.h>
#include <Aria.h>
#include <math.h>

#include "avoid.h"

avoid::avoid() : ArAction("Avoid") {}

bool sonarRangeSort(Sonar a, Sonar b) { return a.range < b.range; }

ArActionDesired * avoid::fire(ArActionDesired d)
{
	desiredState.reset();
	desiredState.setVel(baseSpeed);
	sonars.clear();		//Clear the sonar vector for the new check

	if (first)
	{
		upLeftCD = clock();
		upRightCD = clock();
		leftCD = clock();
		rightCD = clock();
		first = false;
	}

	for (int i = 0; i < 16; i++)	//For all sonars
	{	
		if (myRobot->getSonarRange(i) <= threshold)	//If the sonar's range is below the threshold (close enough to the robot)
		{
			Sonar sonar;
			//Check which direction the sonar is coming from and store in the sonar:
			if ((i >= 2) && (i < 6))		//Front sonars
				sonar.dir = UP;
			else if ((i >= 6) && (i < 9))	//Right sonars
				sonar.dir = RIGHT;
			else if ((i >= 9) && (i < 15))							//Left sonars
				sonar.dir = OTHER;
			else
				sonar.dir = LEFT;
			sonar.range = myRobot->getSonarRange(i);	//Also store the range and sonar number
			sonar.number = i;
			sonars.push_back(sonar);					//Add it to the vector
		}
	}
	if (sonars.size() != 0)		//If there are any sonars to be checked (Only if there are some in range of the threshold)
	{
		std::sort(sonars.begin(), sonars.end(), sonarRangeSort);	//Sort by their ranges
		DIRECTION = prioritise(sonars);								//Function to find best action

		switch (DIRECTION)
		{
			case(UP_LEFT): {
				if (((clock() - upLeftCD) / (double)CLOCKS_PER_SEC) > 3)
				{
					std::cout << "Reversing to the left.\n";	//Reverse to the left
					adjust(-speed, -90);
					upLeftCD = clock();
				}
			}break;

			case(UP_RIGHT): {
				if (((clock() - upRightCD) / (double)CLOCKS_PER_SEC) > 3)
				{
					std::cout << "Reversing to the right.\n";	//Reverse to the right
					adjust(-speed, 90);
					upRightCD = clock();
				}
			}break;
			case(LEFT): {
				if (((clock() - leftCD) / (double)CLOCKS_PER_SEC) > 3)
				{
					std::cout << "Slight adjust against the left.\n";
					adjust(-speed * 0.25, -30);
					leftCD = clock();
				}
			}break;
			case(RIGHT): {
				if (((clock() - rightCD) / (double)CLOCKS_PER_SEC) > 3)
				{
					std::cout << "Slight adjust against the right.\n";
					adjust(-speed * 0.25, 30);
					rightCD = clock();
				}
			}break;
			case(OTHER): {}break;
			default: {std::cerr << "ERROR: Invalid state for avoid\n"; }
		}
		return &desiredState;
	}
	else
	{
		return &desiredState;
	}
}

int avoid::prioritise(std::vector<Sonar> sonars)
{
	//Function to prioritie what to do
	int prio = OTHER;
	bool up = false;
	for (Sonar sonar : sonars)
	{
		if (sonar.dir == UP)
		{
			up = true;
		}
		if (up)
		{
			if (sonar.dir == LEFT)
				return UP_LEFT;
			if (sonar.dir == RIGHT)
				return UP_RIGHT;
		}
		else
		{
			if ((sonar.dir == LEFT) || (sonar.dir == RIGHT))
				return sonar.dir;
		}
	}
	return prio;
}

void avoid::adjust(int speed, int deltaHead)
{
	desiredState.setVel(speed);
	desiredState.setDeltaHeading(deltaHead);
}