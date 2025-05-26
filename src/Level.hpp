#pragma once

#include "Object.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"
#include "Library.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "Particle.hpp"
#include "Time.hpp"
#include "BaseState.hpp"

#include "Objects/CameraView.hpp"
#include "Objects/Chandelier.hpp"
#include "Objects/Destroyable.hpp"
#include "Objects/Door.hpp"
#include "Objects/ElevatorDoor.hpp"
#include "Objects/GlassBox.hpp"
#include "Objects/Gun.hpp"
#include "Objects/Lever.hpp"
#include "Objects/Panel.hpp"
#include "Objects/WarpDoor.hpp"
#include "Objects/LockedWarpDoor.hpp"
#include "Objects/Fan.hpp"
#include "Objects/Button.hpp"
#include "Objects/Lightning.hpp"
#include "Objects/OnShootPower.hpp"
#include "Objects/Darkness.hpp"
#include "Objects/Gravitational.hpp"
#include "Objects/StaticDestroyable.hpp"
#include "Objects/OnPowerDisappear.hpp"
#include "Objects/Rad.hpp"
#include "Objects/Key.hpp"
#include "Objects/MovingPlatformHorizontal.hpp"
#include "Objects/MovingPlatformVertical.hpp"
#include "Objects/DisappearingPlatform.hpp"
#include "Objects/OnPlayerJumpToggle.hpp"
#include "Objects/OnTimerToggle.hpp"
#include "Objects/OnCollisionWarp.hpp"
#include "Objects/DestroyableWall.hpp"
#include "Objects/SlotMachine.hpp"
#include "Objects/OutroTrigger.hpp"
#include "Objects/OnCollisionLoadLevel1.hpp"
#include "Objects/RevolvingDoor.hpp"
#include "Objects/Teddy.hpp"

#include "Enemies/Gonraed.hpp"
#include "Enemies/Gondrae.hpp"

inline std::shared_ptr<Object> GetObjectFromId(uint64_t id) {
	switch (id) {
	case (uint64_t)Object::Id::Default:					 return std::make_shared<Object>();
	case (uint64_t)Object::Id::CameraView:				 return std::make_shared<CameraView>();
	case (uint64_t)Object::Id::ElevatorDoor:			 return std::make_shared<ElevatorDoor>();
	case (uint64_t)Object::Id::Panel:					 return std::make_shared<Panel>();
	case (uint64_t)Object::Id::Destroyable:				 return std::make_shared<Destroyable>();
	case (uint64_t)Object::Id::Door:					 return std::make_shared<Door>();
	case (uint64_t)Object::Id::Chandelier:				 return std::make_shared<Chandelier>();
	case (uint64_t)Object::Id::GlassBox:				 return std::make_shared<GlassBox>();
	case (uint64_t)Object::Id::Gun:						 return std::make_shared<Gun>();
	case (uint64_t)Object::Id::Lever:					 return std::make_shared<Lever>();
	case (uint64_t)Object::Id::WarpDoor:				 return std::make_shared<WarpDoor>();
	case (uint64_t)Object::Id::LockedWarpDoor:			 return std::make_shared<LockedWarpDoor>();
	case (uint64_t)Object::Id::Fan:						 return std::make_shared<Fan>();
	case (uint64_t)Object::Id::Button:					 return std::make_shared<Button>();
	case (uint64_t)Object::Id::Lightning:				 return std::make_shared<Lightning>();
	case (uint64_t)Object::Id::OnShootPower:			 return std::make_shared<OnShootPower>();
	case (uint64_t)Object::Id::Gonraed:					 return std::make_shared<Gonraed>();
	case (uint64_t)Object::Id::Darkness:				 return std::make_shared<Darkness>();
	case (uint64_t)Object::Id::Gravitational:			 return std::make_shared<Gravitational>();
	case (uint64_t)Object::Id::StaticDestroyable:		 return std::make_shared<StaticDestroyable>();
	case (uint64_t)Object::Id::OnPowerDisappear:		 return std::make_shared<OnPowerDisappear>();
	case (uint64_t)Object::Id::Key:						 return std::make_shared<Key>();
	case (uint64_t)Object::Id::Rad:						 return std::make_shared<Rad>();
	case (uint64_t)Object::Id::MovingPlatformHorizontal: return std::make_shared<MovingPlatformHorizontal>();
	case (uint64_t)Object::Id::MovingPlatformVertical:	 return std::make_shared<MovingPlatformVertical>();
	case (uint64_t)Object::Id::DisappearingPlatform:	 return std::make_shared<DisappearingPlatform>();
	case (uint64_t)Object::Id::OnPlayerJumpToggleState0: return std::make_shared<OnPlayerJumpToggleState0>();
	case (uint64_t)Object::Id::OnPlayerJumpToggleState1: return std::make_shared<OnPlayerJumpToggleState1>();
	case (uint64_t)Object::Id::OnTimerToggleState0:		 return std::make_shared<OnTimerToggleState0>();
	case (uint64_t)Object::Id::OnTimerToggleState1:		 return std::make_shared<OnTimerToggleState1>();
	case (uint64_t)Object::Id::OnCollisionWarp:			 return std::make_shared<OnCollisionWarp>();
	case (uint64_t)Object::Id::DestroyableWall:			 return std::make_shared<DestroyableWall>();
	case (uint64_t)Object::Id::SlotMachine:				 return std::make_shared<SlotMachine>();
	case (uint64_t)Object::Id::OutroTrigger:			 return std::make_shared<OutroTrigger>();
	case (uint64_t)Object::Id::OnCollisionLoadLevel1:	 return std::make_shared<OnCollisionLoadLevel1>();
	case (uint64_t)Object::Id::RevolvingDoor:			 return std::make_shared<RevolvingDoor>();
	case (uint64_t)Object::Id::Teddy:					 return std::make_shared<Teddy>();
	default:											 return std::make_shared<Object>();
	};
}

class State;

class Level : public BaseState {
public:
	virtual void Update(Input& input, State& state);

	virtual void Render(Renderer& renderer);

	virtual void LoadFromFile(const char* filename, Renderer& resources);

	void SetLevelIndex(uint8_t index);

protected:
	void AddPlayer(vec2 size);

	void AddGonraed(FloatRect rect);

	void AddGondrae(FloatRect rect);

	void InternalLoad(const char* filename, Renderer& resources);

	void SetCamera(Renderer& renderer);

	Engine m_Engine;

	std::shared_ptr<Player> m_Player;

	std::shared_ptr<Object> m_Climbable;

	std::vector<std::shared_ptr<ElevatorDoor>> m_Elevators;

	std::shared_ptr<ElevatorDoor> m_FirstElevator;

	std::shared_ptr<ElevatorDoor> m_LastElevator;

	std::shared_ptr<OutroTrigger> m_OutroTrigger;

	std::shared_ptr<OnCollisionLoadLevel1> m_Level1Warper;

	std::vector<std::shared_ptr<Darkness>> m_Darkness;

	FloatRect texRect;

	Camera m_Camera;

	uint8_t m_LevelIndex = 0;
};