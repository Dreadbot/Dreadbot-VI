#include "Drive.h"

class GuidedDrive : public Drive
{
public:
	GuidedDrive(double newTime, double newSpeed) : Drive(newTime, newSpeed) {}
	void enter();
	int update();
	void exit();
protected:
	std::shared_ptr<NetworkTable> grip; //*Sigh*... shared_ptrs are WPILib's new thing. Don't blame me!
};
