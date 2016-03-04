#include "Shoot.h"

void Shoot::enter()
{
	autoLog->log("Entering state 'Shoot'!");
}
int Shoot::update()
{
	armUp();
	Wait(0.5);
	armOut();
	Wait(0.49);
	shoot();
	return TIMER_EXPIRED;
}
void Shoot::exit()
{
	autoLog->log("Leaving state 'Shoot'!");
}
