#include "RoboState.h"

//"Dumb" state -- implements the shooter macro
class Shoot : public RoboState
{
public:
	void enter();
	int update();
	void exit();
};
