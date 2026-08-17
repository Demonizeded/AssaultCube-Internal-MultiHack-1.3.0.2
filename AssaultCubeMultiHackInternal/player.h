#pragma once
#include "Vec.h"


//We recreated Entity Memory Layout, to more effectivly read/write data
class Player {
public:
	char pad0004[0x4]; //padding
	Vec3 headPos;
	Vec3 vel;
	char pad001C[0xC];
	Vec3 pos;
	float yaw;
	float pitch;
	char pad003C[0x21];
	bool onGround;
	char pad005E[0x8E];
	int health;
	char pad00F0[0x115];
	char nick[16];
	char pad0215[0xF7];
	int team;
};