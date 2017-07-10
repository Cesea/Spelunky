#ifndef COMMAND_H
#define COMMAND_H

struct Win32RawInputState;

class InputMapper
{
public :
	InputMapper();
	virtual ~InputMapper();

	ControlCommand InterpretRawInput(Win32RawInputState *rawInput);
};



#endif