#include "AutoBot.h"

//Initializes states
AutoBot::AutoBot()
{
	log = Logger::getLog("robolog");

	drive_lowbar = new Drive(PREFS_LOWBAR_TIME, BREACH_SPEED);
	drive_breach = new Drive(PREFS_BREACH_TIME, BREACH_SPEED);
	drive_goal = new Drive(PREFS_GOAL_TIME, BREACH_SPEED);
	rotate_lowbar = new Rotate(PREFS_LOWBAR_ANGLE);
	stopped = new Stopped;
	shoot = new Shoot();
}

//Deletes states
AutoBot::~AutoBot()
{
	delete drive_lowbar;
	delete drive_breach;
	delete drive_goal;
	delete rotate_lowbar;
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
	RoboState::autoLog = Logger::getLog("robolog");
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
	else if (mode == DUMB)
	{
		log->log("Applying state table 'DUMB'");
		defState = drive_lowbar;
		transitionTable[i++] = {drive_lowbar, RoboState::TIMER_EXPIRED, rotate_lowbar};
		transitionTable[i++] = {rotate_lowbar, RoboState::TIMER_EXPIRED, drive_goal};
		transitionTable[i++] = {drive_goal, RoboState::TIMER_EXPIRED, shoot};
		transitionTable[i++] = {shoot, RoboState::TIMER_EXPIRED, stopped};
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
