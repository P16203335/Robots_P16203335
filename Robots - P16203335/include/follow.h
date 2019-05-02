// follow.h
#include <deque>
#include <iostream>
#include <string>

// Component for each part of follow
class FollowComponent {
public:
	std::string name;
	double weight;
	 
	std::deque<double> errors; // Error over time.

	double p; // Gain
	double i; // Gain
	double d; // Gain

	double update(unsigned int cycleTime, bool active, double error = 0.0) {
		if (!active) 
		{ 
			error = 0.0; 
		}

		// Cycle error buffer.
		errors.push_front(error);
		errors.pop_back();
		
		// Proportional
		double proportional = p * error;

		// Integral
		double integral = 0.0;
		for (int i = 0; i < 10; i++) { integral += errors[i]; }
		integral *= (10 * cycleTime) * i;

		// Derivative
		double derivative = ((error - errors[1]) / cycleTime) * d;

		double output = (proportional + integral + derivative) * weight;

		return output;
	}

	FollowComponent(std::string name, double p, double i, double d, double weight = 1.0) : name(name), p(p), i(i), d(d), weight(weight) {
		errors = {
			0.0, 0.0,
			0.0, 0.0,
			0.0, 0.0,
			0.0, 0.0,
			0.0, 0.0
		};
	}
};

class follow : public ArAction // Class action inherits from ArAction
{
public:
	follow(); // Constructor
	virtual ~follow() {}  // Destructor
	virtual ArActionDesired * fire(ArActionDesired d); // Body of the action
	ArActionDesired desiredState; // Holds state of the robot that we wish to action
protected:
	int baseSpeed = 200, speed = 150; // Speed in mm/s
	double deltaHead = 0; // Change in heading
	// Reading
	double front;

	double smaller(double a, double b)
	{
		if (a < b)
			return a;
		else
			return b;
	}

	double bigger(double a, double b)
	{
		if (a > b)
			return a;
		else
			return b;
	}

	// Control variables
	double snapOffset = 2000;
	double goalOffset = 500;

	// output calculation components
	FollowComponent offset = FollowComponent("Offset", 0.5, 0.0001, 100.0, 0.3);
	FollowComponent parallel = FollowComponent("Parallel", 0.5, 0.0001, 100.0, 0.35);
	FollowComponent wide = FollowComponent("Widebeam", 0.5, 0.0001, 100.0, 0.35);
};