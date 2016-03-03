#include "RoboState.h"

//"Dumb" state -- implements the shooter macro
class Shoot : RoboState
{
public:
	void enter();
	int update();
	void exit();
};
