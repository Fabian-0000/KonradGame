#pragma once

#include "Object.hpp"
#include "Animation.hpp"

class Fan;
class MovingPlatformHorizontal;
class MovingPlatformVertical;

class Shot : public Object {
public:
	Shot();

	void Fire(FloatRect playerRect, bool playerFlipped, bool down = false);

	void Update(Input& input) override;

	void Render(Renderer& renderer) override;

	bool IsFlipped();

	uint8_t GetAnimationFrame();

	bool IsAnimationOver();

private:
	Animation m_AnimationShot = Animation(100, 6);

	bool m_Flip = false;

	bool m_Down = false;
};

class Player : public Object {
public:
	bool IsDashing();

	void Update(Input& input) override;

	void OnCollision(const Collider& self, const Collider& other) override;

	void Render(Renderer& renderer) override;

	void RenderOverlay(Renderer& renderer, bool interactOnly = false);

	bool IsInteracting();

	void Warp(vec2 pos);

	void Damage(uint8_t count = 1);

	void Kill();

	bool UnlockDoor();

	void SetRadCount(uint8_t count);

	bool IsStartingJump();

	void CancelJump();

	bool IsDead();

	vec2 defaultSize;

private:
	void ActDash(float dir);

	void ActJump();

	void ActDoubleJump();

	void UpdateDash();

	void UpdateGravity(Input& input);

	void EnableCollision(bool state);

	void UpdateSprite(Input& input);

	uint8_t maxHp = 5;

	int8_t hp = 5;

	uint8_t maxRads = 2;

	uint8_t rads = 0;

	uint8_t keys = 0;

	FloatRect lastCheckpoint;

	bool invincible = false;

	sf::Clock invincibilityClock;

	bool canInteract = false;

	bool controllerInput = false;

	uint8_t warped = 0;

	vec2 lastPosition;

	vec2 dashSpeed = 0.0f;

	vec2 speed = 0.0f;

	vec2 floatingSpeed = 0.0f;

	bool interacting = false;

	bool isGrounded = false;

	bool canDash = true;

	bool canDoubleJump = false;

	enum class States {
		Walk, Sprint, Jump, Sneak, Dash, DoubleJump, Climb
	};

	sf::Clock jumpClock;

	sf::Clock dashCooldown;

	sf::Clock groundLeaveClock;

	sf::Clock shootCooldown;

	bool collision = true;

	bool dashing = false;

	bool inClimbCollider = false;

	bool doubleJump = false;

	bool climbing = false;

	bool floating = false;

	bool sneaking = false;

	bool sprinting = false;

	bool shooting = false;

	bool wasSneaking = false;

	bool canUnsneak = true;

	bool flip = false;

	bool cameraFree = false;

	bool oldCameraFree = cameraFree;

	bool moving = false;

	bool isStartingJump = false;

	FloatRect cameraRect = FloatRect(0, -9, 16, 9);

	FloatRect oldCameraRect = cameraRect;

	Shot shot;

	uint64_t textureIndex2 = 0;

	FloatRect textureRect2;

	Animation m_AnimationIdle = Animation(300, 5);

	Animation m_AnimationWalk = Animation(300, 6);

	Animation m_AnimationSprint = Animation(100, 6);

	Animation m_AnimationSneak = Animation(300, 4);

	Animation m_AnimationShoot = Animation(100, 6);

	Animation m_AnimationClimb = Animation(200, 4);

	Animation m_AnimationDoubleJump = Animation(200, 4);

	Animation m_AnimationOneFrame = Animation(100, 1);

	friend class Level;

	friend class Fan;

	friend class MovingPlatformHorizontal;
	friend class MovingPlatformVertical;
};