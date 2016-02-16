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
	double diff = getYaw() - startRot + dest;
	SmartDashboard::PutNumber("diff", diff);
	if (fabs(diff) > 180.0)
		diff -= diff > 0 ? 180 : -180;
	SmartDashboard::PutNumber("smartdiff", diff);

	SmartDashboard::PutNumber("corrective", -diff / 70.0);
	drive(0, -diff / 70.0);

	if (fabs(diff) < 5.0)
		return TIMER_EXPIRED;
	return NO_UPDATE;
}

void Rotate::exit()
{
	autoLog->log("Leaving state 'Rotate' with rotation of " + std::to_string(getYaw()) + " degrees...");
}
