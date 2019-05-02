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
	sonars.clear();
	for (int i = 0; i < 16; i++)
	{	
		if (myRobot->getSonarRange(i) <= threshold)
		{
			Sonar sonar;
			if ((i >= 2) && (i < 6))
				sonar.dir = UP;
			else if ((i >= 6) && (i < 9))
				sonar.dir = RIGHT;
			else
				sonar.dir = LEFT;
			sonar.range = myRobot->getSonarRange(i);
			sonar.number = i;
			sonars.push_back(sonar);
		}
	}
	if (sonars.size() != 0)
	{
		std::sort(sonars.begin(), sonars.end(), sonarRangeSort);	
		sonars.size() == 1 ? DIRECTION = sonars.front().dir : DIRECTION = prioritise(sonars);
		if (count == 0)
		{
			PAST_DIR = DIRECTION;
			count++;
		}
		else
		{
			DIRECTION == PAST_DIR ? count++ : count = 0;
		}
		if (count > 10)
		{
			DIRECTION = REVERSE;
			desiredState.setDeltaHeading(((rand() % 20) - 10));			
		}
		if (DIRECTION != UP)
		{
			std::cout << "Avoiding wall ";
		}	
		switch (DIRECTION)
		{
			case(RIGHT): {
				std::cout << "right.\n";
				desiredState.setDeltaHeading(8);
			}break;

			case(LEFT): {
				std::cout << "left.\n";
				desiredState.setDeltaHeading(-8);
			}break;

			case(UP_LEFT): {
				std::cout << "above and left.\n";
				desiredState.setVel(-speed);
				desiredState.setDeltaHeading(-90);
			}break;

			case(UP_RIGHT): {
				std::cout << "above and right.\n";
				desiredState.setVel(-speed);
				desiredState.setDeltaHeading(90);
			}break;
			case(REVERSE): {
				std::cout << "NULL : Reversing.\n";
				desiredState.setVel(-speed * 2);
			}
			case(UP): {}break;
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
	int prio = UP;
	int close = 0;
	bool up = false;
	for (Sonar sonar : sonars)
	{
		if (sonar.dir == UP)
		{
			up = true;
			prio = UP_LEFT;
		}
		else if ((sonar.dir == LEFT) || (sonar.dir == RIGHT))
		{
			if (up)
			{
				if (sonar.dir == LEFT)
					return UP_LEFT;
				if (sonar.dir == RIGHT)
					return UP_RIGHT;
			}
			else
			{
				return sonar.dir;
			}
		}		
	}
	return prio;
}