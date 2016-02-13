#include "RoboState.h"
using ADBLib::SimplePID;

class Drive : public RoboState
{
public:
	Drive(double newTime, double newSpeed);
	~Drive();
	void enter();
	int update();
	void exit();
protected:
	double time;
	double speed;
	double startRot;
	Timer* driveTimer;
};
