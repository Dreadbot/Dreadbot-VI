#include "Shoot.h"

void Shoot::enter()
{
	autoLog->log("Entering state 'Shoot'!");
}
int Shoot::update()
{
	armDown();
	Wait(ARMUP_TIME);
	//armOut();					//Commented out in the name of the Holy Low Bar
	//Wait(ARMOUT_SHOOTERDELAY);
	shoot();
	return TIMER_EXPIRED;
}
void Shoot::exit()
{
	autoLog->log("Leaving state 'Shoot'!");
}
