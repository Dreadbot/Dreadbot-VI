#pragma once

#include "WPILib.h"
#include "../ADBLib/src/ADBLib.h"
using namespace ADBLib;

#define DRIVE_KP 0.007
#define DRIVE_KI 0.0
#define DRIVE_KD 0.003

class RoboState : public FSMState
{
public:
	enum retcode {NO_UPDATE = 1, ESTOP, TIMER_EXPIRED};

	virtual void enter() = 0; //Functions inherited from FSMState
	virtual int update() = 0;
	virtual void  exit() = 0;

protected:
	static void shoot();
	static void armUp();
	static void armStop(); //For RAISING the arm!
	static void armDown();
	static void armOut();
	static void armIn();
	static void drive(double transY, double rot);
	static double getYaw();
	static double getYawRate();
	static Log* autoLog;

private:
	static Drivebase* drivebase;
	static SimplePneumatic* shooterPiston;
	static SimplePneumatic* arm;
	static SimplePneumatic* extendArm;
	static AHRS* ahrs;

	friend class AutoBot;
};
