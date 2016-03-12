#include "Drive.h"

Drive::Drive(double newTime, double newSpeed)
{
	time = newTime;
	speed = newSpeed;
	startRot = 0;
	driveTimer = new Timer;
}

Drive::~Drive()
{
	delete driveTimer;
}

void Drive::enter()
{
	if (globalDrive.size() > 0)
	{
		autoLog->log("Drive state OVERRIDING DRIVE TIME of " + to_string(time) + "s with NEW TIME of " + to_string(globalDrive.top()) + "s!");
		time = globalDrive.top();
		globalDrive.pop();
	}

	startRot = getYaw();
	autoLog->log("Entering state 'Drive' for " + to_string(time) + " seconds and rotation of " + to_string(getYaw()) + " degrees!");
	driveTimer->Start();
}

int Drive::update()
{
	double diff = getYaw() - startRot;
	drive(speed, -diff * DRIVE_SCALAR);

	if (driveTimer->Get() >= time)
		return TIMER_EXPIRED;
	if (isColliding() && driveTimer->Get() > COLLISION_DELAY)
	{
		autoLog->log("Drive state detected collision!");
		return COLLISION;
	}

	return NO_UPDATE;
}

void Drive::exit()
{
	autoLog->log("Leaving state 'Drive'!");
	driveTimer->Stop();
	driveTimer->Reset();
}
