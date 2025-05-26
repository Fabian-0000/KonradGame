#include "Player.hpp"
#include "Time.hpp"
#include "Objects/Rad.hpp"
#include "SaveData.hpp"

#define JUMP_SPEED -0.15f
#define DOUBLE_JUMP_SPEED -0.225f
#define GRAVITY_ACC 0.015f

Shot::Shot() {

}

void Shot::Fire(FloatRect playerRect, bool playerFlipped, bool down) {
	m_AnimationShot.Reset();

	transform.rect = playerRect;
	transform.size.w = transform.size.w / 34.0f * 22.0f;
	if (playerFlipped) {
		m_Flip = true;
		transform.position.x = playerRect.x - transform.size.w - 2.0f / 12.0f;
	}
	else {
		m_Flip = false;
		transform.position.x = playerRect.x + playerRect.w + 2.0f / 12.0f;
	}

	if (down) {
		m_Down = true;
		transform.size.w = playerRect.w;
		transform.size.h = playerRect.h / 32.0f * 20.0f;
		if (playerFlipped) {
			transform.position.y = playerRect.y + playerRect.h;
			transform.position.x = playerRect.x;
		}
		else {
			transform.position.y = playerRect.y + playerRect.h;
			transform.position.x = playerRect.x;
		}
	}
	else {
		m_Down = false;
	}
}

void Shot::Update(Input& input) {
	m_AnimationShot.Update();

	if (!m_Down) {
		textureIndex = Renderer::PlayerTextures::Shot;
		textureRect.w = 22;
		textureRect.h = 32;
		textureRect.x = m_AnimationShot.GetIndex() * 22;
		textureRect.y = 0;
	}
	else {
		textureIndex = Renderer::PlayerTextures::ShotDown;
		textureRect.w = 34;
		textureRect.h = 20;
		textureRect.x = m_AnimationShot.GetIndex() * 34;
		textureRect.y = 0;
	}
}

void Shot::Render(Renderer& renderer) {
	if (m_AnimationShot.IsOver()) return;

	sf::RectangleShape object;

	object.setTexture(&renderer.GetPlayerTextures()[textureIndex]);
	object.setTextureRect(ToSFMLIntRect(textureRect));
	object.setScale(sf::Vector2f(m_Flip? -1 : 1, 1));
	object.setOrigin(ToSFMLVec2(transform.size) / 2.0f);

	object.setPosition(ToSFMLVec2(transform.position) + ToSFMLVec2(transform.size) / 2.0f);
	object.setSize(ToSFMLVec2(transform.size));

	renderer.GetRenderTarget().setView(renderer.GetView().GetSFMLView());
	renderer.GetRenderTarget().draw(object);
}

bool Shot::IsFlipped() {
	return m_Flip;
}

uint8_t Shot::GetAnimationFrame() {
	return m_AnimationShot.GetIndex();
}

bool Shot::IsAnimationOver() {
	return m_AnimationShot.IsOver();
}

/////////////////////////////////////////////////////////////////////

bool Player::IsDashing() {
	return dashing;
}

bool Player::IsDead() {
	return (maxHp != 0 && hp <= 0);
}

void Player::Update(Input& input) {
	maxHp = g_SaveData.hpCount;

	m_AnimationIdle.Update();
	m_AnimationShoot.Update();
	m_AnimationSprint.Update();
	m_AnimationWalk.Update();
	m_AnimationSneak.Update();
	m_AnimationClimb.Update();
	m_AnimationDoubleJump.Update();

	if (input.GetLastInputType() == Input::InputType::Controller) controllerInput = true;
	else controllerInput = false;

	if (warped) {
		warped--;
	}

	if (invincible) {
		if (invincibilityClock.getElapsedTime().asMilliseconds() > 3000) {
			invincible = false;
		}
	}

	cameraFree = false;
	interacting = false;

	moving = abs(lastPosition.x - transform.position.x) > 0.0001f;
	lastPosition = transform.position;

	if (input.IsButtonPressed(Input::Button::Interact)) {
		interacting = true;
	}

	if (input.GetDirectionInput().x != 0.0f) {
		flip = input.GetDirectionInput().x < 0.0f;
	}

	if (inClimbCollider && input.GetDirectionInput().y < -0.8) {
		climbing = true;
	}
	if (climbing) {
		if (!inClimbCollider) {
			climbing = false;
			speed = 0.0f;
			canDash = true;
			canDoubleJump = true;
		}

		transform.position += input.GetDirectionInput() * 3.f * Time.DeltaTime();

		if (input.IsButtonPressed(Input::Button::Jump)) {
			speed = 0.0f;
			ActJump();
			jumpClock.restart();
			climbing = false;
			canDash = true;
			canDoubleJump = true;
		}

		isGrounded = false;
		inClimbCollider = false;

		UpdateSprite(input);

		shot.Update(input);

		colliders[3].box = GetColliderRect(FloatRect(0, 0, 1, 1));

		return;
	}

	if (input.IsButtonDown(Input::Button::Sneak) && isGrounded) {
		sneaking = true;
	}
	else if (canUnsneak) sneaking = false;

	if (sneaking && !wasSneaking) {
		for (auto& col : colliders) {
			if (col.tag == Collider::Tags::PlayerTop) {
				col.box = FloatRect(25.f / 68.f + 0.00001f, -1.0f + 5.f / 34.f - 0.01f, 18.f / 68.f - 0.00002f, 1.0f);
			}
		}
		transform.size.y = defaultSize.y / 2.0f;
		transform.position.y += defaultSize.y / 2.0f;
	}

	if (g_SaveData.hasShotgun && !sneaking && !dashing && !doubleJump && input.IsButtonPressed(Input::Button::Shoot) && shootCooldown.getElapsedTime().asMilliseconds() > 700) {
		shooting = true;
		shootCooldown.restart();
		shot.Fire(transform.rect, flip);
		m_AnimationShoot.Reset();
	}

	if (m_AnimationShoot.IsOver()) {
		shooting = false;
	}

	if (!sneaking && wasSneaking) {
		transform.size.y = defaultSize.y;
		transform.position.y -= defaultSize.y / 2.0f;
		for (auto& col : colliders) {
			if (col.tag == Collider::Tags::PlayerTop) {
				col.box = FloatRect(25.f / 68.f + 0.00001f, 2.f / 34.f - 0.005f, 18.f / 68.f - 0.00002f, 0.105f);
			}
		}
	}

	wasSneaking = sneaking;

	if (dashCooldown.getElapsedTime().asMilliseconds() > 500 && input.IsButtonPressed(Input::Button::Dash)) {
		if (input.GetDirectionInput().x > 0) {
			ActDash(0.2f);
			dashCooldown.restart();
		}
		else if (input.GetDirectionInput().x < 0) {
			ActDash(-0.2f);
			dashCooldown.restart();
		}
	}

	UpdateDash();

	isStartingJump = false;

	if (!sneaking && speed.y >= 0.0f && input.IsButtonPressed(Input::Button::Jump) && groundLeaveClock.getElapsedTime().asMilliseconds() < 200) {
		ActJump();
		isStartingJump = true;
		jumpClock.restart();
	}

	if (isGrounded) {
		if (!input.IsButtonDown(Input::Button::DoubleJump)) {
			canDoubleJump = true;
		}
	}
	if (g_SaveData.hasShotgun && !doubleJump && !shooting && !sneaking && input.IsButtonPressed(Input::Button::DoubleJump) && canDoubleJump) {
		dashing = false;
		dashSpeed = 0;
		doubleJump = true;
		ActDoubleJump();
		m_AnimationDoubleJump.Reset();
		shot.Fire(transform.rect, flip, true);
		canDoubleJump = false;
	}

	shot.Update(input);

	UpdateGravity(input);

	float sprint = 1.0f;

	sprinting = false;
	if (!sneaking && input.IsButtonDown(Input::Button::Sprint)) {
		if (abs(input.GetDirectionInput().x) > 0.8f) sprinting = true;
		sprint = 1.5f;
	}

	speed.x += input.GetDirectionInput().x * (isGrounded ? 1.0f : 0.35f) * (sneaking ? 0.35f : 1.0f) * 0.015f * 60.0f * Time.DeltaTime();

	if (speed.x > 0.1f * abs(input.GetDirectionInput().x) * (sneaking ? 0.35f : 1.0f)) {
		speed.x = 0.1f * abs(input.GetDirectionInput().x) * (sneaking ? 0.35f : 1.0f);
	}
	if (speed.x < -0.1f * abs(input.GetDirectionInput().x) * (sneaking ? 0.35f : 1.0f)) {
		speed.x = -0.1f * abs(input.GetDirectionInput().x) * (sneaking ? 0.35f : 1.0f);
	}

	if (speed.x > 0.0f) {
		speed.x -= 0.001f * 60.0f * Time.DeltaTime();
		if (speed.x < 0.0f) {
			speed.x = 0.0f;
		}
	}
	if (speed.x < 0.0f) {
		speed.x += 0.001f * 60.0f * Time.DeltaTime();
		if (speed.x > 0.0f) {
			speed.x = 0.0f;
		}
	}

	if (floating && !dashing && !(doubleJump && speed.y < 0.0f) && (speed.y > 0.0f)) {
		speed = 0;
		canDoubleJump = true;
		canDash = true;

		floatingSpeed.x = std::clamp(floatingSpeed.x, -0.1f, 0.1f);
		floatingSpeed.y = std::clamp(floatingSpeed.y, -0.1f, 0.1f);

		transform.position += input.GetDirectionInput() * 2.f * Time.DeltaTime() + floatingSpeed * 60.0f * Time.DeltaTime();
		UpdateSprite(input);
		shot.Update(input);

		floatingSpeed = 0.0f;

		floating = false;
	}
	else {
		transform.position.x += (speed.x * sprint + dashSpeed.x) * 60.0f * Time.DeltaTime();
		transform.position.y += speed.y * 60.0f * Time.DeltaTime();
	}

	if (shooting) {
		FloatRect rect = GetColliderRect(shot.transform.rect);

		FloatRect colliderRects[] = {
			FloatRect(2, 9, 5, 5),
			FloatRect(2, 9, 8, 5),
			FloatRect(2, 7, 18, 9),
			FloatRect(7, 7, 13, 9),
			FloatRect(10, 9, 9, 5),
			FloatRect(14, 10, 3, 3),
		};

		FloatRect colRect = colliderRects[shot.GetAnimationFrame()];

		if (shot.IsFlipped()) {
			rect.x += rect.w * (22 - colRect.x - colRect.w) / 22.0f;
			rect.y += rect.h * colRect.y / 32.0f;
		}
		else {
			rect.x += rect.w * colRect.x / 22.0f;
			rect.y += rect.h * colRect.y / 32.0f;
		}
		rect.w *= colRect.w / 22.0f;
		rect.h *= colRect.h / 32.0f;
		colliders[3].box = rect;
	}
	else if (doubleJump && !shot.IsAnimationOver()) {
		FloatRect rect = GetColliderRect(shot.transform.rect);

		FloatRect colliderRects[] = {
			FloatRect(22, 2, 5, 5),
			FloatRect(22, 2, 5, 8),
			FloatRect(20, 2, 9, 18),
			FloatRect(20, 7, 9, 13),
			FloatRect(22, 10, 5, 9),
			FloatRect(23, 14, 3, 3),
		};

		FloatRect colRect = colliderRects[shot.GetAnimationFrame()];

		if (shot.IsFlipped()) {
			rect.x += rect.w * (34 - colRect.x - colRect.w) / 34.0f;
			rect.y += rect.h * colRect.y / 20.0f;
		}
		else {
			rect.x += rect.w * colRect.x / 34.0f;
			rect.y += rect.h * colRect.y / 20.0f;
		}
		rect.w *= colRect.w / 34.0f;
		rect.h *= colRect.h / 20.0f;
		colliders[3].box = rect;
	}
	else {
		colliders[3].box = GetColliderRect(FloatRect(0, 0, 1, 1));
	}

	UpdateSprite(input);

	isGrounded = false;
	inClimbCollider = false;
	canUnsneak = true;
	canInteract = false;
}

#include "Objects/Fan.hpp"

void Player::OnCollision(const Collider& self, const Collider& other) {
	if (!collision) {
		return;
	}

	/*if (other.tag == Collider::Tags::CameraFree && self.tag == Collider::Tags::Player) {
		cameraFree = true;
	}

	if (other.tag == Collider::Tags::CameraLocked && self.tag == Collider::Tags::Player) {
		cameraRect = other.parent->GetBoxColliderRect(other);
	}*/

	if (other.tag == Collider::Tags::Interact) {
		canInteract = true;
	}

	if (other.parent->id == Object::Id::CameraView && self.tag == Collider::Tags::Player) {
		cameraRect = other.parent->transform.rect;
	}

	if (sneaking && self.tag == Collider::Tags::PlayerTop && other.tag == Collider::Tags::Default && !other.isTrigger) {
		canUnsneak = false;
	}

	if (other.tag == Collider::Tags::Climb && self.tag == Collider::Tags::Player) {
		inClimbCollider = true;
	}

	if ((other.tag == Collider::Tags::AirStreamNorth ||
		 other.tag == Collider::Tags::AirStreamEast ||
		 other.tag == Collider::Tags::AirStreamSouth ||
		 other.tag == Collider::Tags::AirStreamWest
		) 
		&& ((Fan*)other.parent)->IsActivated()
		&& self.tag == Collider::Tags::Player
		&& !isGrounded) {
		floating = true;
	}

	if (self.tag == Collider::Tags::PlayerGround && !other.isTrigger) {
		if (speed.y >= 0.0f) isGrounded = true;
		if (speed.y >= 0.0f) {
			speed.y = 0.0f;
		}
		groundLeaveClock.restart();
		if (!dashing) canDash = true;
	}

	if (self.tag == Collider::Tags::PlayerTop && !other.isTrigger) {
		if (speed.y < 0.0f) {
			speed.y = 0.0f;
		}
	}

	if ((self.tag == Collider::Tags::Player || self.tag == Collider::Tags::PlayerGround) && other.tag == Collider::Tags::Damage && !invincible) {
		hp--;
		invincibilityClock.restart();
		invincible = true;
	}

	if (self.tag == Collider::Tags::Player && other.tag == Collider::Tags::CheckPoint) {
		lastCheckpoint = other.parent->GetBoxColliderRect(other);
	}

	if ((self.tag == Collider::Tags::Player || self.tag == Collider::Tags::PlayerGround) && other.tag == Collider::Tags::Death) {
		hp--;
		invincibilityClock.restart();
		invincible = true;
		Warp(vec2(lastCheckpoint.x + lastCheckpoint.w / 2.0f - transform.size.w / 2.0f, lastCheckpoint.y + lastCheckpoint.h - transform.size.h));
	}

	if (other.parent->id == Object::Id::Key) {
		keys++;
		other.parent->Delete();
	}

	if (self.tag == Collider::Tags::Player && other.parent->id == Object::Id::Rad && !((Rad*)other.parent)->IsLocked() && interacting) {
		rads++;
		other.parent->Delete();
	}
}

bool Player::IsInteracting() {
	return interacting;
}

bool Player::UnlockDoor() {
	if (keys > 0) { keys--; return true; }
	return false;
}

bool Player::IsStartingJump() {
	return isStartingJump;
}

void Player::CancelJump() {
	speed.y = 0.0f;
	isGrounded = true;
}

void Player::SetRadCount(uint8_t count) {
	maxRads = count;
}

void Player::Warp(vec2 pos) {
	transform.position = pos;
	warped = 2;
}

void Player::ActDash(float dir) {
	if (canDash) {
		dashSpeed.x = dir;
		speed.y = 0.0f;
		canDash = false;
		dashing = true;
	}
}

void Player::ActJump() {
	speed.y = JUMP_SPEED;
	isGrounded = false;
}

void Player::ActDoubleJump() {
	speed.y = DOUBLE_JUMP_SPEED;
	isGrounded = false;
}

void Player::UpdateDash() {
	if (dashSpeed.x > 0.0f) {
		dashSpeed.x -= 0.75f * Time.DeltaTime();
		if (dashSpeed.x < 0.0f) {
			dashSpeed.x = 0.0f;
		}
	}

	else if (dashSpeed.x < 0.0f) {
		dashSpeed.x += 0.75f * Time.DeltaTime();
		if (dashSpeed.x > 0.0f) {
			dashSpeed.x = 0.0f;
		}
	}

	else {
		dashing = false;
	}
}

void Player::UpdateGravity(Input& input) {
	const float gravityAcc = GRAVITY_ACC;

	if (dashSpeed.x == 0) {
		if (input.IsButtonDown(Input::Button::Jump) && jumpClock.getElapsedTime().asMilliseconds() < 200) {
			speed.y += gravityAcc / 5.f * Time.DeltaTime() * 60.0f;
		}
		else speed.y += gravityAcc * Time.DeltaTime() * 60.0f;
	}

	if (input.GetDirectionInput().x == 0.0f && isGrounded) {
		speed.y = 0.00f;
	}

	if (speed.y > 0.2f) {
		speed.y = 0.2f;
	}
}

void Player::EnableCollision(bool state) {
	for (auto& col : colliders) {
		if (col.tag == Collider::Tags::Player) {
			col.isTrigger = !state;
		}
	}
	collision = state;
}

void Player::UpdateSprite(Input& input) {
	Animation* animation = &m_AnimationIdle;
	textureIndex = Renderer::PlayerTextures::IdleLegs;
	textureIndex2 = Renderer::PlayerTextures::IdleBody;

	if (moving) {
		textureIndex = Renderer::PlayerTextures::WalkLegs;
		textureIndex2 = Renderer::PlayerTextures::WalkBody;
		animation = &m_AnimationWalk;

		animation->SetSpeed(1.f / std::exp(abs(input.GetDirectionInput().x) - 1) * 150);
	}

	if (sprinting && moving) {
		textureIndex = Renderer::PlayerTextures::SprintLegs;
		textureIndex2 = Renderer::PlayerTextures::SprintBody;

		animation = &m_AnimationSprint;
	}

	if (!isGrounded) {
		textureIndex = Renderer::PlayerTextures::JumpLegs;
		textureIndex2 = Renderer::PlayerTextures::JumpBody;

		animation = &m_AnimationOneFrame;
	}

	if (dashing) {
		textureIndex = Renderer::PlayerTextures::DashLegs;
		textureIndex2 = Renderer::PlayerTextures::DashBody;

		animation = &m_AnimationOneFrame;
	}

	if (doubleJump) {
		if (m_AnimationDoubleJump.IsOver()) {
			doubleJump = false;
		}
		else {

			textureIndex = Renderer::PlayerTextures::DoubleJump;

			animation = &m_AnimationDoubleJump;
		}
	}
	else {
		m_AnimationDoubleJump.Reset();
	}

	if (sneaking) {
		textureIndex = Renderer::PlayerTextures::Sneak;

		animation = &m_AnimationSneak;
	}

	if (climbing) {
		textureIndex = Renderer::PlayerTextures::Climb;

		animation = &m_AnimationClimb;

		animation->SetSpeed(1.0f / abs(input.GetDirectionInput().y) * 150);
	}
	else {
		m_AnimationClimb.Reset();
	}

	if (!moving) {
		m_AnimationWalk.Reset();
		m_AnimationSneak.Reset();
	}
	if (!sprinting) {
		m_AnimationSprint.Reset();
	}

	uint64_t index = animation->GetIndex();

	textureRect.w = 34 * 2;
	textureRect.h = 32 * 2;
	textureRect.x = index * 34 * 2;
	textureRect.y = 0;

	if (climbing) {
		textureRect.w = 33;
		textureRect.h = 32;
		textureRect.x = index * 33;
		textureRect.y = 0;
	}

	textureRect2 = textureRect;

	if (!m_AnimationShoot.IsOver() && shooting) {
		textureIndex2 = Renderer::PlayerTextures::Shoot;
		textureRect2.x = m_AnimationShoot.GetIndex() * 34 * 2;

		if (animation == &m_AnimationWalk || animation == &m_AnimationSprint) {
			if (index == 1 || index == 4) {
				textureRect2.y = -2;
			}
		}
	}

	if (animation == &m_AnimationIdle) {
		textureRect.x = 0;
	}
}

void Player::Damage(uint8_t count) {
	if (!invincible) {
		hp -= count;
		invincibilityClock.restart();
		invincible = true;
	}
}

void Player::Kill() {

}

void Player::Render(Renderer& renderer) {
	if (warped) {
		vec2 target = vec2(transform.position.x + transform.size.x / 2.0f, transform.position.y + transform.size.h);
		renderer.GetCamera().SetSize(vec2(16, 9));
		renderer.GetCamera().Target(target);
		renderer.GetCamera().StayInRect(true);
		renderer.GetCamera().StopSmoothTransition();
		renderer.GetCamera().Update();
	}

	if (invincible) {
		if ((invincibilityClock.getElapsedTime().asMilliseconds() / 100) % 2) {
			return;
		}
	}

	sf::RectangleShape object;

	if (invincible) {
		if (invincibilityClock.getElapsedTime().asMilliseconds() < 200) {
			object.setFillColor(sf::Color(255, 80, 80, 255));
		}
	}

	object.setTexture(&renderer.GetPlayerTextures()[textureIndex]);
	object.setTextureRect(ToSFMLIntRect(textureRect));
	object.setScale(sf::Vector2f(flip ? -1 : 1, 1));
	object.setOrigin(ToSFMLVec2(transform.size) / 2.0f);

	object.setPosition(ToSFMLVec2(transform.position) + ToSFMLVec2(transform.size) / 2.0f);
	object.setSize(ToSFMLVec2(transform.size));

	renderer.GetRenderTarget().setView(renderer.GetView().GetSFMLView());
	renderer.GetRenderTarget().draw(object);

	if (!sneaking && !climbing && !doubleJump) {
		object.setTexture(&renderer.GetPlayerTextures()[textureIndex2]);
		object.setTextureRect(ToSFMLIntRect(textureRect2));
		renderer.GetRenderTarget().draw(object);
	}

	if (shooting || doubleJump) {
		shot.Render(renderer);
	}

	//**************************************************************************************//

	/*sf::RectangleShape object2;

	object2.setFillColor(sf::Color(0, 0, 255, 50));
	object2.setPosition(ToSFMLVec2(GetBoxColliderRect(colliders[0]).xy));
	object2.setSize(ToSFMLVec2(GetBoxColliderRect(colliders[0]).wh));

	renderer.GetRenderTarget().setView(renderer.GetView().GetSFMLView());
	renderer.GetRenderTarget().draw(object2);

	object2.setFillColor(sf::Color(255, 0, 0, 50));
	object2.setPosition(ToSFMLVec2(GetBoxColliderRect(colliders[1]).xy));
	object2.setSize(ToSFMLVec2(GetBoxColliderRect(colliders[1]).wh));

	renderer.GetRenderTarget().setView(renderer.GetView().GetSFMLView());
	renderer.GetRenderTarget().draw(object2);

	object2.setFillColor(sf::Color(255, 0, 0, 50));
	object2.setPosition(ToSFMLVec2(GetBoxColliderRect(colliders[2]).xy));
	object2.setSize(ToSFMLVec2(GetBoxColliderRect(colliders[2]).wh));

	renderer.GetRenderTarget().setView(renderer.GetView().GetSFMLView());
	renderer.GetRenderTarget().draw(object2);

	object2.setFillColor(sf::Color(0, 255, 0, 50));
	object2.setPosition(ToSFMLVec2(GetBoxColliderRect(colliders[3]).xy));
	object2.setSize(ToSFMLVec2(GetBoxColliderRect(colliders[3]).wh));

	renderer.GetRenderTarget().setView(renderer.GetView().GetSFMLView());
	renderer.GetRenderTarget().draw(object2);*/
}

void Player::RenderOverlay(Renderer& renderer, bool interactOnly) {
	enum TextureRects {
		Heart, EmptyHeart, Key, Rad, EmptyRad, ControllerInteract, KeyboardInteract
	};

	FloatRect textureRects[] = {
		FloatRect(0, 0, 12, 11),
		FloatRect(11, 0, 12, 11),
		FloatRect(0, 10, 12, 13),
		FloatRect(0, 22, 12, 12),
		FloatRect(11, 22, 12, 12),
		FloatRect(0, 34, 13, 13),
		FloatRect(14, 35, 11, 11),
	};
	
	sf::RectangleShape object;
	object.setTexture(&renderer.GetPlayerTextures()[Renderer::PlayerTextures::Overlay]);

	if (!interactOnly) {
		for (uint8_t i = 0; i < maxHp; i++) {
			uint8_t rectIndex = i < hp ? Heart : EmptyHeart;

			object.setTextureRect(ToSFMLIntRect(textureRects[rectIndex]));
			object.setPosition(ToSFMLVec2(vec2(3 + i * textureRects[rectIndex].w, 3)));
			object.setSize(ToSFMLVec2(textureRects[rectIndex].wh));

			renderer.GetOverlayRenderTarget().draw(object);
		}

		for (uint8_t i = 0; i < maxRads; i++) {
			uint8_t rectIndex = maxRads - i - 1 < rads ? Rad : EmptyRad;

			object.setTextureRect(ToSFMLIntRect(textureRects[rectIndex]));
			object.setPosition(ToSFMLVec2(vec2(192 - 3 - (maxRads - i) * textureRects[rectIndex].w, 3)));
			object.setSize(ToSFMLVec2(textureRects[rectIndex].wh));

			renderer.GetOverlayRenderTarget().draw(object);
		}

		for (uint8_t i = 0; i < keys; i++) {
			object.setTextureRect(ToSFMLIntRect(textureRects[Key]));
			object.setPosition(ToSFMLVec2(vec2(192 - 3 - (keys - i) * textureRects[Key].w, 17)));
			object.setSize(ToSFMLVec2(textureRects[Key].wh));

			renderer.GetOverlayRenderTarget().draw(object);
		}
	}

	if (canInteract) {
		uint8_t rectIndex = controllerInput ? ControllerInteract : KeyboardInteract;

		object.setTextureRect(ToSFMLIntRect(textureRects[rectIndex]));
		object.setPosition(ToSFMLVec2(vec2(192 - 3 - textureRects[rectIndex].w, 108 - 3 - textureRects[rectIndex].h)));
		object.setSize(ToSFMLVec2(textureRects[rectIndex].wh));

		renderer.GetOverlayRenderTarget().draw(object);
	}
}