#pragma once

#include "WPILib.h"
#include "../ADBLib/src/ADBLib.h"
using ADBLib::FSMState;
using ADBLib::SimplePneumatic;
using ADBLib::Drivebase;
using ADBLib::Logger;
using ADBLib::Log;

class RoboState : public FSMState
{
public:
	enum retcode {NO_UPDATE, ESTOP};

	virtual int update() = 0; //Functions inherited from FSMState
	virtual void enter() = 0;
	virtual void  exit() = 0;

protected:
	static void shoot();
	static void armUp();
	static void armDown();
	static void armOut();
	static void armIn();
	static void drive(double transY, double rot);
	static Log* autoLog;

private:
	static Drivebase* drivebase;
	static SimplePneumatic* shooterPiston;
	static SimplePneumatic* arm;
	static SimplePneumatic* extendArm;

	friend class AutoBot;
};
