#include "AutoBot.h"

//Initializes states
AutoBot::AutoBot()
{
	log = Logger::getLog("sysLog");

	drive_breach = new Drive(BREACH_TIME, BREACH_SPEED);
	guidedDrive = new GuidedDrive(20, BREACH_SPEED); //20 second time because guided driving should end with a collision
	rotate = new Rotate(90.0);
	stopped = new Stopped;
	shoot = new Shoot();
}

//Deletes states
AutoBot::~AutoBot()
{
	delete drive_breach;
	delete guidedDrive;
	delete rotate;
	delete shoot;
	delete stopped;
}

//Provides hardware dependencies to RoboState
void AutoBot::init(Drivebase* newDB, SimplePneumatic* newShooterPiston,
				SimplePneumatic* newArm, SimplePneumatic* newExtendArm, AHRS* ahrs)
{
	RoboState::drivebase = newDB;
	RoboState::shooterPiston = newShooterPiston;
	RoboState::arm = newArm;
	RoboState::extendArm = newExtendArm;
	RoboState::ahrs = ahrs;
	RoboState::autoLog = Logger::getLog("sysLog");
}

//Switches modes, changing the state table in the process.
void AutoBot::switchMode(autonModes mode)
{
	RoboState* defState = nullptr;
	int i = 0;

	if (mode == BREACH)
	{
		log->log("Applying state table 'BREACH'!");
		defState = drive_breach;
		transitionTable[i++] = {drive_breach, RoboState::TIMER_EXPIRED, stopped};
		transitionTable[i++] = END_STATE_TABLE;
	}
	else if (mode == GUIDED)
	{
		log->log("Applying state table 'GUIDED'!");
		defState = drive_breach;
		transitionTable[i++] = {drive_breach, RoboState::TIMER_EXPIRED, guidedDrive};
		transitionTable[i++] = {guidedDrive, RoboState::COLLISION, stopped/*shoot*/};
	//	transitionTable[i++] = {shoot, RoboState::TIMER_EXPIRED, stopped};
		transitionTable[i++] = END_STATE_TABLE;
	}
	else //if (mode == NOP || mode == FULL)
	{
		log->log("Applying state table 'NOP'");
		defState = stopped;
		transitionTable[i++] = END_STATE_TABLE;
	}

	fsm.init(transitionTable, defState);
}

void AutoBot::update()
{
	fsm.update();
}
