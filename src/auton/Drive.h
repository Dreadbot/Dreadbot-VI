#pragma once
#include "RoboState.h"

class Drive : public RoboState
{
public:
	Drive(double newTime, double newSpeed);
	~Drive();
	virtual void enter();
	virtual int update();
	virtual void exit();
protected:
	double time;
	double speed;
	double startRot;
	Timer* driveTimer;
};
