#include "Stopped.h"

int Stopped::update()
{
	return NO_UPDATE;
}
void Stopped::enter()
{
	autoLog->log("Entering state 'Stopped'!");
	drive(0, 0);
}
void Stopped::exit()
{
	autoLog->log("Leaving state 'Stopped'!");
}
