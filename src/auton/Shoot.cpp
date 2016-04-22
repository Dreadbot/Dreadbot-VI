#include "Shoot.h"

void Shoot::enter()
{
	autoLog->log("Entering state 'Shoot'!");
}
int Shoot::update()
{
	if (PREFS_HIGH_SHOT)
	{
		armUp();
		Wait(ARMUP_TIME);
		armOut();
		Wait(0.49); //Copied from Robot.cpp, it's the "shooter macro"
	}
	else
	{
		armDown();
		Wait(ARMUP_TIME);
	}
	shoot();
	return TIMER_EXPIRED; //durrrrr
}
void Shoot::exit()
{
	autoLog->log("Leaving state 'Shoot'!");
}
