// follow.h
#include <deque>
#include <iostream>
#include <string>

// Signatures
class FollowComponent {
public:
	std::string m_name;
	double m_weight;
	 
	std::deque<double> m_errors; // Error over time.
	double m_pGain; // Gain
	double m_iGain; // Gain
	double m_dGain; // Gain

	double update(unsigned int cycleTime, bool active, double error = 0.0) {
		if (!active) { error = 0.0; }

		// Cycle error buffer.
		m_errors.push_front(error);
		m_errors.pop_back();

		/*std::cout << m_name << ":" << std::endl;*/

		// Proportional
		double proportional = m_pGain * error;
		/*std::cout << "Proportional: " << proportional << std::endl; */

		// Integral
		double integral = 0.0;
		for (int i = 0; i < 10; i++) { integral += m_errors[i]; }
		integral *= (10 * cycleTime) * m_iGain;
		/*std::cout << "Integral: " << integral << std::endl;*/

		// Derivative
		double derivative = ((error - m_errors[1]) / cycleTime) * m_dGain;
		/*std::cout << "Derivative: " << derivative << std::endl;*/

		double output = (proportional + integral + derivative) * m_weight;
		/*std::cout << "Output: " << output << std::endl << std::endl;
		std::cout << m_name << " Output: " << output << std::endl;*/

		return output;
	}

	FollowComponent(std::string name, double pGain, double iGain, double dGain, double weight = 1.0) : m_name(name), m_pGain(pGain), m_iGain(iGain), m_dGain(dGain), m_weight(weight) {
		m_errors = {
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
	double deltaHeading = 0; // Change in heading
	// Reading
	double lTopSonar, lTMidSonar, lBMidSonar, lBotSonar;
	double rTopSonar, rTMidSonar, rBMidSonar, rBotSonar;
	double frontSonar;

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
	double desiredOffset = 500;

	// output calculation components
	FollowComponent m_offsetComp = FollowComponent("Offset Component", 0.5, 0.0001, 100.0, 0.3);
	FollowComponent m_parallelComp = FollowComponent("Parallel Component", 0.5, 0.0001, 100.0, 0.35);
	FollowComponent m_wideComp = FollowComponent("Widebeam Component", 0.5, 0.0001, 100.0, 0.35);
};