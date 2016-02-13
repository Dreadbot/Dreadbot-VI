#include "../../ADBLib/src/ADBLib.h"
#include "RoboState.h"
#include "Stopped.h"
using ADBLib::FSMTransition;
using ADBLib::FiniteStateMachine;
using ADBLib::Log;
using ADBLib::Logger;
using ADBLib::Drivebase;
using ADBLib::SimplePneumatic;

class AutoBot
{
public:
	enum autonModes {NOP, FULLAUTON};

	AutoBot();
	~AutoBot();
	void init(Drivebase* newDB, SimplePneumatic* newShooterPiston,
				SimplePneumatic* newArm, SimplePneumatic* newExtendArm);
	void switchMode(autonModes mode);
	void update();
	void reset() {} //@TODO Implement AutoBot::reset()

private:
	Log* log;
	FiniteStateMachine fsm;
	FSMTransition transitionTable[15];

	//States
	Stopped* stopped;
};
