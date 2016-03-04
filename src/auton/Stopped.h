#include "RoboState.h"

class Stopped : public RoboState
{
public:
	void enter();
	int update();
	void exit();
};
