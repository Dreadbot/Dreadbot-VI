#include "AutoBot.h"

//Initializes states
AutoBot::AutoBot()
{
	log = Logger::getLog("sysLog");
	stopped = new Stopped;
}

//Deletes states
AutoBot::~AutoBot()
{
	delete stopped;
}

//Provides hardware dependencies to RoboState
void AutoBot::init(Drivebase* newDB, SimplePneumatic* newShooterPiston,
				SimplePneumatic* newArm, SimplePneumatic* newExtendArm)
{
	RoboState::drivebase = newDB;
	RoboState::shooterPiston = newShooterPiston;
	RoboState::arm = newArm;
	RoboState::extendArm = newExtendArm;
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
		transitionTable[i++] = {stopped, RoboState::NO_UPDATE, stopped};
		transitionTable[i++] = END_STATE_TABLE;
	}
	else if (mode == FULLAUTON)
	{
		log->log("Applying state table 'FULLAUTON'");
		defState = stopped;
		transitionTable[i++] = {stopped, RoboState::NO_UPDATE, stopped};
		transitionTable[i++] = END_STATE_TABLE;
	}

	fsm.init(transitionTable, defState);
}

void AutoBot::update()
{
	fsm.update();
}
