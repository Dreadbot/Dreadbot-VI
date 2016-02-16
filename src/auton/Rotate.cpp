#include "Rotate.h"

Rotate::Rotate(double relativeDegrees)
{
	dest = relativeDegrees;
	startRot = 0;
}

void Rotate::enter()
{
	autoLog->log("Entering state 'Rotate' and rotating from " + std::to_string(getYaw()) + " to " + std::to_string(getYaw() + dest) + " degrees...");
	startRot = getYaw();
}

int Rotate::update()
{
	double fdest = dest + startRot; //@TODO Move this into a class member, calculated at enter()
	if (fdest > 360.0)
		fdest -= 360.0;

	//THIS COST 3 HOURS
	double diff = getYaw() - fdest;
	if (diff > 180.0)
		diff = 360.0 - diff;
	if (diff < -180.0)
		diff = 360 + diff;

	drive(0, -diff / 90.0);

	SmartDashboard::PutNumber("diff", diff);
	if (fabs(diff) <= 3.0)
		return TIMER_EXPIRED;
	return NO_UPDATE;
}

void Rotate::exit()
{
	autoLog->log("Leaving state 'Rotate' with rotation of " + std::to_string(getYaw()) + " degrees...");
}
