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
	drive(0, 0);
	driveTimer->Stop();
	driveTimer->Reset();
	Wait(0.3);
}
