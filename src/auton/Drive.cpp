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
	if (globalDriveTime != 0.0)
	{
		autoLog->log("Drive state OVERRIDING DRIVE TIME of " + to_string(time) + "s with NEW TIME of " + to_string(globalDriveTime) + "s!");
		time = globalDriveTime;
		globalDriveTime = 0.0;
	}

	startRot = getYaw();
	autoLog->log("Entering state 'Drive' for " + to_string(time) + " seconds and rotation of " + to_string(getYaw()) + " degrees!");
	driveTimer->Start();
}

int Drive::update()
{
	double diff = getYaw() - startRot;
	drive(speed, -diff / 18.0);

	if (driveTimer->Get() >= time)
		return TIMER_EXPIRED;
	if (isColliding())
	{
		autoLog->log("GuidedDrive state detected collision!");
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
