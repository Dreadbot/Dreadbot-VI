#include "RoboState.h"

//RoboState static members
Drivebase* RoboState::drivebase = nullptr;
SimplePneumatic* RoboState::shooterPiston = nullptr;
SimplePneumatic* RoboState::arm = nullptr;
SimplePneumatic* RoboState::extendArm = nullptr;
AHRS* RoboState::ahrs = nullptr;
Log* RoboState::autoLog = nullptr;
stack<double> RoboState::globalDrive;	//@TODO: Exterminate usage of stacks in autonomous...
stack<double> RoboState::globalRotate;

void RoboState::shoot()
{
	if (shooterPiston == nullptr)
		return;
	shooterPiston->set(1); //Out
	Wait(SHOOT_TIME);
	shooterPiston->set(0); //Automatically retracts
}

void RoboState::armUp()
{
	if (arm != nullptr)
		arm->set(1);
}

void RoboState::armStop()
{
	if (arm != nullptr)
		arm->set(0);
}

void RoboState::armDown()
{
	if (arm != nullptr)
		arm->set(-1);
}

void RoboState::armOut()
{
	if (extendArm != nullptr)
		extendArm->set(1);
}

void RoboState::armIn()
{
	if (extendArm != nullptr)
		extendArm->set(-1);
}

void RoboState::drive(double transY, double rot)
{
	if (drivebase != nullptr)
		drivebase->drive(0, transY, rot); //tX, tY, r
}

double RoboState::getYaw()
{
	if (ahrs != nullptr)
		return ahrs->GetYaw();
	return 0;
}

double RoboState::getYawRate()
{
	if (ahrs != nullptr)
		return ahrs->GetRawGyroY();
	return 0;
}

/*
 * https://github.com/kauailabs/navxmxp/blob/master/roborio/c%2B%2B/navXMXP_CPP_CollisionDetection/src/Robot.cpp
 * THIS FUNCTION IS BEST USED IF CALLED REGULARLY OVER SHORT TIME INCREMENTS!
 */

bool RoboState::isColliding()
{
	if (ahrs == nullptr)
		return false;

	static double lXAccel = 0.0, lYAccel = 0.0; //Only initialized once
	static Timer timer;
	static bool timerRunning = false;
	if (!timerRunning) //WPI timers don't have a way of telling you if they're already running
	{
		timer.Start();
		timerRunning = false;
	}

	if (timer.Get() > COLLISION_TIMEOUT)
	{
		timer.Stop();
		timer.Reset(); //Not sure if all these function calls are needed, but for safety's sake, f**k timers.
		timer.Start();
		lXAccel = ahrs->GetWorldLinearAccelX();
		lYAccel = ahrs->GetWorldLinearAccelY();
		return false; //Since the timeout has been exceeded, return NO COLLISION.
	}

	double xAccel = ahrs->GetWorldLinearAccelX(), yAccel = ahrs->GetWorldLinearAccelY();
	double jerkX = xAccel - lXAccel, jerkY = yAccel - lYAccel;
	lXAccel = xAccel;
	lYAccel = yAccel;
	if (sqrt((jerkX * jerkX) + (jerkY * jerkY)) > COLLISION_THRESHOLD_DELTAG)
		return true;
	return false;
}

