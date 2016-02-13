#include "AutoBot.h"

//Initializes states
AutoBot::AutoBot()
{
	log = Logger::getLog("sysLog");
	stopped = new Stopped;
	drive = new Drive(2.5, 0.6);
}

//Deletes states
AutoBot::~AutoBot()
{
	delete stopped;
	delete drive;
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

	if (mode == NOP)
	{
		log->log("Applying state table 'NOP'");
		defState = stopped;
		transitionTable[i++] = END_STATE_TABLE;
	}
	else if (mode == DRIVE)
	{
		log->log("Applying state table 'DRIVE'");
		defState = drive;
		transitionTable[i++] = {drive, RoboState::TIMER_EXPIRED, stopped};
		transitionTable[i++] = END_STATE_TABLE;
	}
	else if (mode == FULLAUTON)
	{
		log->log("Applying state table 'FULLAUTON'");
		defState = stopped;
		transitionTable[i++] = END_STATE_TABLE;
	}

	fsm.init(transitionTable, defState);
}

void AutoBot::update()
{
	fsm.update();
}
