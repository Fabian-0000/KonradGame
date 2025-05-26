#pragma once
#include "../Object.hpp"
#include "../Animation.hpp"

class ElevatorDoor : public Object {
public:
	bool PlayerEnter();

	void LockDoors(bool val);

	bool DoorsLocked();

	void Update(Input& input) override;

	void Render(Renderer& renderer) override;

	void OnCollision(const Collider& self, const Collider& other) override;

	enum class State {
		Closed, Opening, Open, Closing
	};

	void SetState(State state);

private:
	Animation m_AnimationElevatorOpen = Animation(500, 3);

	State m_State = State::Closed;

	sf::Clock m_ElevatorDoorTimer;

	bool m_PlayerEnter = false;

	bool m_DoorsLocked = false;
};