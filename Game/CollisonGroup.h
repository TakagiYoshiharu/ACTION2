#pragma once
enum CollisionGroup
{
	enCollisionGroup_Player = 1 << 0,  // 0001
	enCollisionGroup_Enemy = 1 << 1,  // 0010
	enCollisionGroup_Ground = 1 << 2,  // 0100
	enCollisionGroup_World=1<<3,
	enCollisionGroup_Sensor=1<<4,
	enCollisionGroup_Enemy2 = 1 << 5,
};

