#include "stdafx.h"
#include "Command.h"

#include "Win32Input.h"

InputMapper::InputMapper()
{
}

InputMapper::~InputMapper()
{
}

ControlCommand InputMapper::InterpretRawInput(Win32RawInputState * rawInput)
{
	ControlCommand result = {};

	if (rawInput->keyboard.left.down)
	{
		result.horizontal = Command::MoveLeft;
		result.fire = true;
	}
	else if (rawInput->keyboard.right.down)
	{
		result.horizontal = Command::MoveRight;
		result.fire = true;
	}

	if (rawInput->keyboard.up.down)
	{
		result.vertical = Command::MoveUp;
		result.fire = true;
	}
	else if (rawInput->keyboard.up.released)
	{
		result.vertical = Command::None;
		result.fire = true;
	}
	else if (rawInput->keyboard.down.down)
	{
		result.vertical = Command::MoveDown;
		result.fire = true;
	}
	else if (rawInput->keyboard.down.released)
	{
		result.vertical = Command::None;
		result.fire = true;
	}

	if (rawInput->keyboard.shift.pressed)
	{
		result.dash = Command::DashOn;
	}
	else if (rawInput->keyboard.shift.released)
	{
		result.dash = Command::DashOff;
	}

	//점프 커맨드 입력
	if (rawInput->keyboard.z.pressed)
	{
		result.jump = Command::Jump;
		result.fire = true;
	}
	else if (rawInput->keyboard.z.released)
	{
		result.jump = Command::JumpOff;
		result.fire = true;
	}


	if (rawInput->keyboard.s.pressed)
	{
		result.action = Command::UseBomb;
		result.fire = true;
	}
	else if (rawInput->keyboard.a.pressed)
	{
		result.action = Command::UseRope;
		result.fire = true;
	}
	else if (rawInput->keyboard.x.pressed)
	{
		result.action = Command::Attack;
		result.fire = true;
	}
	else if (rawInput->keyboard.c.pressed)
	{
		result.action = Command::Dig;
		result.fire = true;
	}
	else if (rawInput->keyboard.space.pressed)
	{
		result.action = Command::GoExit;
		result.fire = true;
	}

	return result;
}
