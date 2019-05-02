#pragma once
class Sonar 
{
private:
public:
	int number;
	int dir;
	double range;
	bool distant = false;
	double getAngle()
	{
		for (int i = 0; i < 3; i++)
		{
			if(number == (i+1))
				return (50 - (20*i));
			if (number == (i+4))
				return (-10 - (20 * i));
			if(number == (i+9))
				return (-130 - (20 * i));
			if (number == (i + 12))
				return (170 - (20 * i));
		}
		if ((number == 0) || (number == 15))
			return 90;
		if ((number == 7) || (number == 8))
			return -90;
	}
};