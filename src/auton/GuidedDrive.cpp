#include "GuidedDrive.h"

void GuidedDrive::enter()
{
	if (globalDrive.size() > 0)
	{
		autoLog->log("GuidedDrive state OVERRIDING DRIVE TIME of " + to_string(time) + "s with NEW TIME of " + to_string(globalDrive.top()) + "s!");
		time = globalDrive.top();
		globalDrive.pop();
	}

	startRot = getYaw();
	autoLog->log("Entering state 'GuidedDrive' for " + to_string(time) + " seconds and rotation of " + to_string(getYaw()) + " degrees!");
	armDown(); //The arm starts high
	Wait(ARMUP_PULSETIME);
	armStop();
	driveTimer->Start();
}

int GuidedDrive::update()
{
	//Returns a shared_ptr object.
	auto grip = NetworkTable::GetTable("grip"); //Returns an std::vector! DO NOT USE sizeof(container)/sizeof(element)!
	auto areas = grip->GetNumberArray("targetsCam0/area", llvm::ArrayRef<double>());

	//If the target goes out of view of the camera, the robot will continue driving, just with no correction.
	double diff = 0;
	if (areas.size() != 0)
	{
		//Select the best target (defined by target with largest area) by index. std::vector<>::iterators won't work here.
		unsigned int bestTarget = 0;
		for (unsigned int i = 0; i < areas.size(); i++)
			if (areas[i] > areas[bestTarget])
				bestTarget = i;

		diff = (CAM_XSIZE / 2.0) - grip->GetNumberArray("targetsCam0/centerX", llvm::ArrayRef<double>())[bestTarget];
		diff *= CAM_ANGRESO;

		//Use vision to keep the center of the target between the 1/4 and 3/4 marks
		double bestY = grip->GetNumberArray("targetsCam0/centerY", llvm::ArrayRef<double>())[bestTarget];
		if (bestY > (3 * CAM_YSIZE / 4.0))
			armDown();
		else if (bestY < CAM_YSIZE / 4.0)
			armUp();
		else
			armStop();
	}
	//The camera is INVERTED!
	drive(/*speed*/0.0, -diff * ROTATE_SCALAR);

	if (driveTimer->Get() >= time)
		return TIMER_EXPIRED;
	if (isColliding() && driveTimer->Get() > COLLISION_DELAY)
	{
		autoLog->log("GuidedDrive state detected collision!");
		return COLLISION;
	}

	return NO_UPDATE;
}

void GuidedDrive::exit()
{
	autoLog->log("Leaving state 'GuidedDrive'!");
	driveTimer->Stop();
	driveTimer->Reset();
}

