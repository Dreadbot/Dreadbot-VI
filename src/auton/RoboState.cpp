#include "RoboState.h"

//RoboState static members
Drivebase* RoboState::drivebase = nullptr;
SimplePneumatic* RoboState::shooterPiston = nullptr;
SimplePneumatic* RoboState::arm = nullptr;
SimplePneumatic* RoboState::extendArm = nullptr;
AHRS* RoboState::ahrs = nullptr;
Log* RoboState::autoLog = nullptr;
double RoboState::globalDriveTime = 0.0;

void RoboState::shoot()
{
	if (shooterPiston == nullptr)
		return;
	shooterPiston->set(1); //Out
	Wait(0.5);
	shooterPiston->set(0); //Automatically retracts
}

void RoboState::armUp()
{
	if (arm != nullptr)
		arm->set(1);
}

void RoboState::armStop()
{
	if (arm != nullptr)
		arm->set(0);
}

void RoboState::armDown()
{
	if (arm != nullptr)
		arm->set(-1);
}

void RoboState::armOut()
{
	if (extendArm != nullptr)
		extendArm->set(1);
}

void RoboState::armIn()
{
	if (extendArm != nullptr)
		extendArm->set(-1);
}

void RoboState::drive(double transY, double rot)
{
	if (drivebase != nullptr)
		drivebase->drive(0, transY, rot); //tX, tY, r
}

double RoboState::getYaw()
{
	if (ahrs != nullptr)
		return ahrs->GetYaw();
	return 0;
}

double RoboState::getYawRate()
{
	if (ahrs != nullptr)
		return ahrs->GetRawGyroY();
	return 0;
}
