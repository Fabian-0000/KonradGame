#pragma once

#include "Vector2.hpp"
#include "Rect.hpp"

struct Triangle {
	vec2 point[3];
};

struct Circle {
	vec2 center;
	float radius;
};

enum class ColliderType {
	BoxCollider,
	CircleCollider,
	TriangleCollider,
};

class Object;

struct Collider {
public:
	Collider() {}

	Collider(const Collider& col);

	ColliderType type;

	union {
		FloatRect box;
		Triangle triangle;
		Circle circle;
	};

	Object* parent = nullptr;

	bool isTrigger = false;

	bool enabled = true;

	enum Tags {
		Default,
		Ground,
		Climb,
		Player,
		PlayerGround,
		PlayerTop,
		CameraLocked,
		CameraFree,
		Interact,
		PlayerShot,
		GroundCheck,
		Dashable,
		ElevatorScreen,
		PowerOff,
		PowerOn,
		AirStreamNorth,
		AirStreamEast,
		AirStreamSouth,
		AirStreamWest,
		Damage,
		Death,
		HighRange,
		LowRange,
		Light,
		TorchLight,
		CheckPoint,
		Lane,
		Warp
	};

	uint64_t tag = 0;
};