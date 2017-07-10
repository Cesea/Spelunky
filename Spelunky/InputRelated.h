#ifndef INPUT_RELATED_H
#define INPUT_RELATED_H


struct DigitalButton
{
	bool down;
	bool pressed;
	bool released;
};

struct Win32Keyboard
{
	DigitalButton up;
	DigitalButton down;
	DigitalButton left;
	DigitalButton right;
	DigitalButton space;

	DigitalButton w;
	DigitalButton s;
	DigitalButton a;
	DigitalButton d;

	DigitalButton e;
};

struct GameInput
{
	DigitalButton up;
	DigitalButton down;
	DigitalButton left;
	DigitalButton right;

	DigitalButton shootUp;
	DigitalButton shooDown;
	DigitalButton shooLeft;
	DigitalButton shooRight;

	DigitalButton useItem;
	DigitalButton useBomb;
};




#endif