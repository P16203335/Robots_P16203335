// Signatures

class forward : public ArAction // Class action inherits from ArAction
{
public:
	forward(); // Constructor
	virtual ~forward() {}  // Destructor
	virtual ArActionDesired * fire(ArActionDesired d); // Body of the action
	ArActionDesired desiredState; // Holds state of the robot that we wish to action
protected:
	int speed; // Speed of the robot in mm/s
};
