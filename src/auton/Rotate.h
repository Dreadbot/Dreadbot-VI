#include "RoboState.h"
using ADBLib::SimplePID;

class Rotate : public RoboState
{
public:
	Rotate(double relativeDegrees);
	void enter();
	int update();
	void exit();
protected:
	double dest;
	double startRot;
};
