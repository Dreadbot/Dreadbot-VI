#include <WPILib.h>
#include "DreadbotDIO.h"

/**
 * Auton mode selector using a "10-switch", adapted from Dreadbot-V
 * Thanks to Mr. King for this code!
 *
 * @return The autonomous mode as defined by AutoBot. If the switch isn't on #'s 1 or 2, the mode is NOP (even for FULL).
 */
AutoBot::autonModes getAutonMode(void)
{
	DigitalInput *bit0 = new DigitalInput(7);
	DigitalInput *bit1 = new DigitalInput(8);
	DigitalInput *bit2 = new DigitalInput(9);
	AutoBot::autonModes mode = AutoBot::NOP;
	int sw = 0;

	if (!bit0->Get()) { sw |= (0x01 << 0); }
	if (!bit1->Get()) { sw |= (0x01 << 1); }
	if (!bit2->Get()) { sw |= (0x01 << 2); }

	delete bit0;
	delete bit1;
	delete bit2;

	if      (sw == 0)  mode = AutoBot::NOP;
	else if (sw == 1)  mode = AutoBot::BREACH;
	else if (sw == 2)  mode = AutoBot::GUIDED;
	else if (sw == 3)  mode = AutoBot::FULL;
	else if (sw == 4)  mode = AutoBot::NOP;
	else if (sw == 5)  mode = AutoBot::NOP;
	else if (sw == 6)  mode = AutoBot::NOP;

	return mode;
}
