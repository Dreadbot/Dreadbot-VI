#include "RoboState.h"

class Stopped : public RoboState
{
public:
	int update();
	void enter();
	void exit();
};
