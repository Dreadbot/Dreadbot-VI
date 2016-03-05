#include "Shoot.h"

void Shoot::enter()
{
	autoLog->log("Entering state 'Shoot'!");
}
int Shoot::update()
{
	armUp();
	Wait(ARMUP_TIME);
	armOut();
	Wait(ARMOUT_SHOOTERDELAY);
	shoot();
	return TIMER_EXPIRED;
}
void Shoot::exit()
{
	autoLog->log("Leaving state 'Shoot'!");
}
