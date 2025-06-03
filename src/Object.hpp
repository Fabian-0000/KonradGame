#pragma once

#include "Transform.hpp"
#include "Collider.hpp"
#include "Input.hpp"
#include "Renderer.hpp"

#include <vector>
#include <memory>

enum class ObjectType {
	Static,
	Dynamic,
	HalfDynamic
};

class Object {
public:
	Object() {}

	virtual void Update(Input& input) {}

	virtual void OnCollision(const Collider& self, const Collider& other) {}

	virtual void Render(Renderer& renderer);

	Transform transform;

	std::vector<Collider> colliders;

	bool hasTexture = false;
	size_t textureIndex = 0;
	FloatRect textureRect;

	Color color = Color(255, 255, 255, 255);

	ObjectType type = ObjectType::Static;

	enum class Id {
		Default,
		Player,
		CameraView,
		ElevatorDoor,
		Panel,
		Destroyable,
		Door,
		Chandelier,
		GlassBox,
		Gun,
		Lever,
		Rad,
		WarpDoor,
		LockedWarpDoor,
		Fan,
		Button,
		Lightning,
		OnShootPower,
		Gonraed,
		Gondrae,
		Darkness,
		Gravitational,
		StaticDestroyable,
		Key,
		OnPowerDisappear,
		MovingPlatformHorizontal,
		MovingPlatformVertical,
		DisappearingPlatform,
		OnPlayerJumpToggleState0,
		OnPlayerJumpToggleState1,
		OnTimerToggleState0,
		OnTimerToggleState1,
		OnCollisionWarp,
		DestroyableWall,
		SlotMachine,
		OutroTrigger,
		OnCollisionLoadLevel1,
		RevolvingDoor,
		Teddy
	};

	Id id = Id::Default;

	FloatRect GetBoxColliderRect(const Collider& collider);

	Circle GetCircleColliderCircle(const Collider& collider);

	FloatRect GetColliderRect(FloatRect rect);

	void ColliderUpdate(std::vector<std::shared_ptr<Object>>& objects);

	void Collide(Object& object);

	void Delete();

	bool exists = true;
};