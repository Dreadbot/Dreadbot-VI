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
	armUp();
	Wait(ARMUP_PULSETIME);
	armStop();
	driveTimer->Start();
}

int GuidedDrive::update()
{
	//Returns a shared_ptr object.
	auto grip = NetworkTable::GetTable("grip"); //Returns an std::vector! DO NOT USE sizeof(container)/sizeof(element)!
	auto areas = grip->GetNumberArray("targetsCam0/area", llvm::ArrayRef<double>());
	double diff = 0;
	if (areas.size() != 0)
	{
		unsigned int bestTarget = 0;

		for (unsigned int i = 0; i < areas.size(); i++)
			if (areas[i] > areas[bestTarget])
				bestTarget = i;

		diff = (CAM_XSIZE / 2.0) - grip->GetNumberArray("targetsCam0/centerX", llvm::ArrayRef<double>())[bestTarget];
		diff *= CAM_ANGRESO;
	}
	drive(speed, diff * ROTATE_SCALAR);

	if (driveTimer->Get() >= time)
		return TIMER_EXPIRED;
	if (isColliding())
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

