#pragma once

enum KEYSTATE
{
	ON,
	OFF
};

enum KEYKIND
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	SPACE,
	ESC,
	LSHIFT,
	RSHIFT,

	MOUSE_RIGHT,
	MOUSE_LEFT,

	TAB,

	KEYMAX
};

bool pushKeyState(int state, int DIK);