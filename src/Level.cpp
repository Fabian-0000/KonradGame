#include "Level.hpp"
#include "Collision.hpp"
#include "State.hpp"
#include "SaveData.hpp"

#include <fstream>

#include <json.hpp>

using namespace nlohmann;

FloatRect ConvertRect(FloatRect object, FloatRect rect) {
	vec2 pos = (rect.xy - object.xy) / object.wh;
	vec2 size = rect.wh / object.wh;

	return FloatRect(pos, size);
}

vec2 ConvertPos(FloatRect object, vec2 pos) {
	return vec2(pos - object.xy) / object.wh;
}

void Level::Update(Input& input, State& state) {
	m_Engine.Update(input);

	if (m_Player->IsDead()) {
		state.Set(State::States::Elevator);
	}

	if (m_FirstElevator.get() != m_LastElevator.get()) {
		if (m_LastElevator->PlayerEnter()) {
			if (g_SaveData.level <= m_LevelIndex) {
				g_SaveData.level = m_LevelIndex + 1;
			}
			WriteSaveData("../data/savefile.json");
		}
	}

	for (auto door : m_Elevators) {
		if (door->PlayerEnter()) {
			state.Set(State::States::Elevator);
		}
	}

	if (m_OutroTrigger.get() && m_OutroTrigger->IsActivated()) {
		state.Set(State::States::Outro);
	}
}

void Level::Render(Renderer& renderer) {
	SetCamera(renderer);

	for (std::shared_ptr<Object> obj : m_Engine.m_Objects) {
		bool render = true;
		for (std::shared_ptr<Darkness>& ptr : m_Darkness) {
			if (ptr.get() == obj.get()) { render = false; break; }
		}
		if (render && obj->exists) obj->Render(renderer);
	}

	m_Player->Render(renderer);

	for (std::shared_ptr<Darkness>& ptr : m_Darkness) {
		ptr->Render(renderer);
	}

	m_Player->RenderOverlay(renderer);
}

void Level::SetCamera(Renderer& renderer) {
	renderer.GetCamera().SetRect(m_Player->cameraRect);

	renderer.GetCamera().SetSize(vec2(16, 9));

	if (m_Player->cameraFree) {
		renderer.GetCamera().StayInRect(false);
	}
	else {
		renderer.GetCamera().StayInRect(true);
	}

	if (m_Player->cameraFree != m_Player->oldCameraFree) {
		renderer.GetCamera().DoSmoothTransition();
	}
	if (!m_Player->cameraFree) {
		if (m_Player->cameraRect != m_Player->oldCameraRect) {
			renderer.GetCamera().DoSmoothTransition();
		}
	}

	vec2 target = vec2(m_Player->transform.position.x + m_Player->transform.size.x / 2.0f, m_Player->transform.position.y + m_Player->transform.size.h);

	renderer.GetCamera().Target(target);

	renderer.GetCamera().Update();

	m_Player->oldCameraFree = m_Player->cameraFree;

	m_Player->oldCameraRect = m_Player->cameraRect;
}

void Level::SetLevelIndex(uint8_t index) {
	m_LevelIndex = index;
}

void Level::AddPlayer(vec2 size) {
	Collider groundCheck;
	groundCheck.type = ColliderType::BoxCollider;
	groundCheck.box = FloatRect(25.f / 68.f + 0.0001f, 0.9f, 18.f / 68.f - 0.0002f, 0.105f);
	groundCheck.isTrigger = true;
	groundCheck.tag = Collider::Tags::PlayerGround;

	Collider ceilingCheck;
	ceilingCheck.type = ColliderType::BoxCollider;
	ceilingCheck.box = FloatRect(25.f / 68.f + 0.0001f, 2.f / 34.f - 0.005f, 18.f / 68.f - 0.0002f, 0.105f);
	ceilingCheck.isTrigger = true;
	ceilingCheck.tag = Collider::Tags::PlayerTop;

	Collider defBox;
	defBox.type = ColliderType::BoxCollider;
	defBox.box = FloatRect(25.f / 68.f, 2.f / 34.f, 18.f / 68.f, 32.f / 34.f);
	defBox.tag = Collider::Tags::Player;

	Collider shot;
	shot.type = ColliderType::BoxCollider;
	shot.box = FloatRect(0.5f, 0.5f, 0.0f, 0.0f);
	shot.tag = Collider::Tags::PlayerShot;
	shot.isTrigger = true;

	m_Player = std::make_shared<Player>();
	m_Player->transform.rect = FloatRect(2, -10, size.w, size.h);
	m_Player->defaultSize = vec2(size.w, size.h);
	defBox.parent = m_Player.get();
	shot.parent = m_Player.get();
	ceilingCheck.parent = m_Player.get();
	groundCheck.parent = m_Player.get();
	m_Player->colliders.push_back(defBox);
	m_Player->colliders.push_back(groundCheck);
	m_Player->type = ObjectType::Dynamic;
	m_Player->colliders.push_back(ceilingCheck);
	m_Player->colliders.push_back(shot);
	m_Player->hasTexture = true;
	m_Player->id = Object::Id::Player;

	m_Engine.AddObject(m_Player);
}

void Level::AddGonraed(FloatRect rect) {
	vec2 size = vec2(2.833333f, 2.666666f);

	vec2 pos = vec2(rect.x + rect.w / 2.0f - size.w / 2.0f, rect.y + rect.h - size.h);

	Collider groundCheck;
	groundCheck.type = ColliderType::BoxCollider;
	groundCheck.box = FloatRect(25.f / 68.f + 0.0001f, 0.9f, 18.f / 68.f - 0.0002f, 0.105f);
	groundCheck.isTrigger = true;
	groundCheck.tag = Collider::Tags::GroundCheck;

	Collider defBox;
	defBox.type = ColliderType::BoxCollider;
	defBox.box = FloatRect(25.f / 68.f, 2.f / 34.f, 18.f / 68.f, 32.f / 34.f);
	defBox.tag = Collider::Tags::Damage;

	Collider highRange;
	highRange.type = ColliderType::BoxCollider;
	highRange.box = FloatRect(-4 / size.w, 0.1f, 8 / size.w + 1, 0.9f);
	highRange.tag = Collider::Tags::HighRange;
	highRange.isTrigger = true;

	Collider lowRange;
	lowRange.type = ColliderType::BoxCollider;
	lowRange.box = FloatRect(0.3f / size.w, 0.1f, -0.6f / size.w + 1, 0.9f);
	lowRange.tag = Collider::Tags::LowRange;
	lowRange.isTrigger = true;

	std::shared_ptr<Gonraed> gonraed = std::make_shared<Gonraed>();

	defBox.parent = gonraed.get();
	highRange.parent = gonraed.get();
	lowRange.parent = gonraed.get();
	groundCheck.parent = gonraed.get();

	gonraed->transform.rect = FloatRect(pos.x, pos.y, size.w, size.h);
	gonraed->colliders.push_back(defBox);
	gonraed->colliders.push_back(highRange);
	gonraed->colliders.push_back(lowRange);
	gonraed->colliders.push_back(groundCheck);
	gonraed->type = ObjectType::Dynamic;
	gonraed->hasTexture = true;
	gonraed->id = Object::Id::Gonraed;

	m_Engine.AddObject(gonraed);
}

void Level::AddGondrae(FloatRect rect) {
	vec2 size = vec2(2.833333f, 2.666666f);

	vec2 pos = vec2(rect.x + rect.w / 2.0f - size.w / 2.0f, rect.y + rect.h - size.h);

	Collider groundCheck;
	groundCheck.type = ColliderType::BoxCollider;
	groundCheck.box = FloatRect(25.f / 68.f + 0.0001f, 0.9f, 18.f / 68.f - 0.0002f, 0.105f);
	groundCheck.isTrigger = true;
	groundCheck.tag = Collider::Tags::GroundCheck;

	Collider defBox;
	defBox.type = ColliderType::BoxCollider;
	defBox.box = FloatRect(25.f / 68.f, 2.f / 34.f, 18.f / 68.f, 32.f / 34.f);
	defBox.tag = Collider::Tags::Damage;

	Collider highRange;
	highRange.type = ColliderType::BoxCollider;
	highRange.box = FloatRect(-4 / size.w, 0.1f, 8 / size.w + 1, 0.9f);
	highRange.tag = Collider::Tags::HighRange;
	highRange.isTrigger = true;

	Collider lowRange;
	lowRange.type = ColliderType::BoxCollider;
	lowRange.box = FloatRect(0.3f / size.w, 0.1f, -0.6f / size.w + 1, 0.9f);
	lowRange.tag = Collider::Tags::LowRange;
	lowRange.isTrigger = true;

	std::shared_ptr<Gondrae> gondrae = std::make_shared<Gondrae>();

	defBox.parent = gondrae.get();
	highRange.parent = gondrae.get();
	lowRange.parent = gondrae.get();
	groundCheck.parent = gondrae.get();

	gondrae->transform.rect = FloatRect(pos.x, pos.y, size.w, size.h);
	gondrae->colliders.push_back(defBox);
	gondrae->colliders.push_back(highRange);
	gondrae->colliders.push_back(lowRange);
	gondrae->colliders.push_back(groundCheck);
	gondrae->type = ObjectType::Dynamic;
	gondrae->hasTexture = true;
	gondrae->id = Object::Id::Gondrae;

	m_Engine.AddObject(gondrae);
}

#include "Collision.hpp"

void Level::LoadFromFile(const char* filename, Renderer& renderer) {
	AddPlayer(vec2(2.833333f, 2.666666f));

	InternalLoad(filename, renderer);

	// Set the player to the first Elevator in the level

	uint8_t radCount = 0;

	uint64_t i = 0;
	uint64_t firstElevator = 0;
	uint64_t lastElevator = 0;
	bool isElevator = false;
	for (std::shared_ptr<Object> obj : m_Engine.m_Objects) {
		if (obj->id == Object::Id::ElevatorDoor) {
			std::shared_ptr<Object> oldElevator = m_Engine.m_Objects[firstElevator];
			if (oldElevator->id == Object::Id::ElevatorDoor) {
				if (obj->transform.position.x < oldElevator->transform.position.x) {
					firstElevator = i;
				}
				if (obj->transform.position.x > oldElevator->transform.position.x) {
					lastElevator = i;
				}
			}
			else {
				firstElevator = i;
				lastElevator = i;
			}
			m_Elevators.push_back(std::dynamic_pointer_cast<ElevatorDoor>(obj));
			isElevator = true;
		}

		if (obj->id == Object::Id::Rad) {
			radCount++;
		}

		if (obj->id == Object::Id::Darkness) {
			m_Darkness.push_back(std::dynamic_pointer_cast<Darkness>(obj));
		}

		if (obj->id == Object::Id::OutroTrigger) {
			m_OutroTrigger = std::dynamic_pointer_cast<OutroTrigger>(obj);
		}

		if (obj->id == Object::Id::OnCollisionLoadLevel1) {
			m_Level1Warper = std::dynamic_pointer_cast<OnCollisionLoadLevel1>(obj);
		}

		i++;
	}

	m_Player->SetRadCount(radCount);

	if (isElevator) {
		m_FirstElevator = std::dynamic_pointer_cast<ElevatorDoor>(m_Engine.m_Objects[firstElevator]);
		m_LastElevator = std::dynamic_pointer_cast<ElevatorDoor>(m_Engine.m_Objects[lastElevator]);


		//((ElevatorDoor*)m_Engine.m_Objects[firstElevator].get())->SetState(ElevatorDoor::State::Open);

		FloatRect elevRect = m_FirstElevator->transform.rect;
		vec2 target = vec2(elevRect.x + elevRect.w / 2.0f - m_Player->transform.rect.w / 2.0f, elevRect.y + elevRect.h - m_Player->transform.rect.h);
		m_Player->transform.rect.xy = target;
	}

	FloatRect playerRect = m_Player->transform.rect;

	for (std::shared_ptr<Object>& obj : m_Engine.m_Objects) {
		if (obj->id != Object::Id::CameraView) {
			continue;
		}

		FloatRect collider = obj->colliders[0].box;

		if (RectCollide(playerRect.xy, playerRect.wh, 0, 0, 0, 0, collider.xy, collider.wh, 0, 0, &obj->transform.position, &obj->transform.size)) {
			vec2 target = vec2(m_Player->transform.position.x + m_Player->transform.size.x / 2.0f, m_Player->transform.position.y + m_Player->transform.size.h - 2.833333f + 2.833333f / 3.0f);
			renderer.GetCamera().SetStartRect(obj->transform.rect);
			renderer.GetCamera().SetSize(vec2(16, 9));
			renderer.GetCamera().Target(target);
			renderer.GetCamera().StayInRect(true);
			renderer.GetCamera().Update();

			m_Player->cameraFree = false;
			m_Player->cameraRect = obj->transform.rect;

			m_Player->oldCameraFree = m_Player->cameraFree;
			m_Player->oldCameraRect = m_Player->cameraRect;
		}
	}
}

void Level::InternalLoad(const char* filename, Renderer& renderer) {
	std::ifstream inputFile;
	inputFile.open(filename);

	json in;

	if (inputFile.is_open()) {
		in = json::parse(inputFile, nullptr, false);
		inputFile.close();
	}

	if (!in.contains("Type")) {
		return;
	}

	if (in["Type"] != "LevelData") {
		return;
	}

	if (in.contains("Resources") && in["Resources"].contains("Sprites") && in["Resources"]["Sprites"].is_array()) {
		for (auto& path : in["Resources"]["Sprites"]) {
			if (path.is_string()) {
				renderer.GetTextures().push_back(sf::Texture());
				renderer.GetTextures()[renderer.GetTextures().size() - 1].loadFromFile(path);
			}
		}
	}

	if (in.contains("Objects") && in["Objects"].is_array()) {
		for (auto& obj : in["Objects"]) {
			uint64_t objectId = 0;
			bool on = false;
			if (obj.contains("Type") && obj["Type"].is_string()) {
				std::string id = obj["Type"];
				objectId = (uint64_t)Object::Id::Default;

				if      (id == "CameraView")						objectId = (uint64_t)Object::Id::CameraView;
				else if (id == "Chandelier")						objectId = (uint64_t)Object::Id::Chandelier;
				else if (id == "Destroyable")						objectId = (uint64_t)Object::Id::Destroyable;
				else if (id == "Door")								objectId = (uint64_t)Object::Id::Door;
				else if (id == "ElevatorDoor")						objectId = (uint64_t)Object::Id::ElevatorDoor;
				else if (id == "GlassBox")							objectId = (uint64_t)Object::Id::GlassBox;
				else if (id == "Gun")								objectId = (uint64_t)Object::Id::Gun;
				else if (id == "Lever")								objectId = (uint64_t)Object::Id::Lever;
				else if (id == "Panel")								objectId = (uint64_t)Object::Id::Panel;
				else if (id == "Rad")								objectId = (uint64_t)Object::Id::Rad;
				else if (id == "WarpDoor")							objectId = (uint64_t)Object::Id::WarpDoor;
				else if (id == "LockedWarpDoor")					objectId = (uint64_t)Object::Id::LockedWarpDoor;
				else if (id == "Fan")								objectId = (uint64_t)Object::Id::Fan;
				else if (id == "ButtonOff" || id == "ButtonOn")		objectId = (uint64_t)Object::Id::Button;
				else if (id == "Lightning")							objectId = (uint64_t)Object::Id::Lightning;
				else if (id == "OnShootPower")						objectId = (uint64_t)Object::Id::OnShootPower;
				else if (id == "Gonraed")							objectId = (uint64_t)Object::Id::Gonraed;
				else if (id == "Gondrae")							objectId = (uint64_t)Object::Id::Gondrae;
				else if (id == "Darkness")							objectId = (uint64_t)Object::Id::Darkness;
				else if (id == "Gravitational")						objectId = (uint64_t)Object::Id::Gravitational;
				else if (id == "StaticDestroyable")					objectId = (uint64_t)Object::Id::StaticDestroyable;
				else if (id == "OnPowerDisappear")					objectId = (uint64_t)Object::Id::OnPowerDisappear;
				else if (id == "Key")								objectId = (uint64_t)Object::Id::Key;
				else if (id == "MovingPlatformHorizontal")			objectId = (uint64_t)Object::Id::MovingPlatformHorizontal;
				else if (id == "MovingPlatformVertical")			objectId = (uint64_t)Object::Id::MovingPlatformVertical;
				else if (id == "DisappearingPlatform")			    objectId = (uint64_t)Object::Id::DisappearingPlatform;
				else if (id == "OnPlayerJumpToggleState0")			objectId = (uint64_t)Object::Id::OnPlayerJumpToggleState0;
				else if (id == "OnPlayerJumpToggleState1")			objectId = (uint64_t)Object::Id::OnPlayerJumpToggleState1;
				else if (id == "OnTimerToggleState0")			    objectId = (uint64_t)Object::Id::OnTimerToggleState0;
				else if (id == "OnTimerToggleState1")			    objectId = (uint64_t)Object::Id::OnTimerToggleState1;
				else if (id == "OnCollisionWarp")			        objectId = (uint64_t)Object::Id::OnCollisionWarp;
				else if (id == "DestroyableWall")			        objectId = (uint64_t)Object::Id::DestroyableWall;
				else if (id == "SlotMachine")						objectId = (uint64_t)Object::Id::SlotMachine;
				else if (id == "OutroTrigger")						objectId = (uint64_t)Object::Id::OutroTrigger;
				else if (id == "OnCollisionLoadLevel1")				objectId = (uint64_t)Object::Id::OnCollisionLoadLevel1;
				else if (id == "RevolvingDoor")						objectId = (uint64_t)Object::Id::RevolvingDoor;
				else if (id == "Teddy")								objectId = (uint64_t)Object::Id::Teddy;

				if (id == "ButtonOn") {
					on = true;
				}
			}

			std::shared_ptr<Object> object;

			if (objectId == (uint64_t)Object::Id::Gonraed) {
				if (obj.contains("Rect") && obj["Rect"].is_array() && obj["Rect"].size() == 4) {
					auto& i = obj["Rect"];
					if (i[0].is_number_float() && i[1].is_number_float() && i[2].is_number_float() && i[3].is_number_float()) {
						FloatRect rect = FloatRect((float)obj["Rect"][0], (float)obj["Rect"][1], (float)obj["Rect"][2], (float)obj["Rect"][3]);
						AddGonraed(rect);
						continue;
					}
				}
			}

			if (objectId == (uint64_t)Object::Id::Gondrae) {
				if (obj.contains("Rect") && obj["Rect"].is_array() && obj["Rect"].size() == 4) {
					auto& i = obj["Rect"];
					if (i[0].is_number_float() && i[1].is_number_float() && i[2].is_number_float() && i[3].is_number_float()) {
						FloatRect rect = FloatRect((float)obj["Rect"][0], (float)obj["Rect"][1], (float)obj["Rect"][2], (float)obj["Rect"][3]);
						AddGondrae(rect);
						continue;
					}
				}
			}

			object = GetObjectFromId(objectId);

			if (on) {
				((Button*)object.get())->SetAcitvated(true);
			}

			object->id = (Object::Id)objectId;

			if (obj.contains("Rect") && obj["Rect"].is_array() && obj["Rect"].size() == 4) {
				auto& i = obj["Rect"];
				if (i[0].is_number_float() && i[1].is_number_float() && i[2].is_number_float() && i[3].is_number_float()) {
					object->transform.rect = FloatRect((float)obj["Rect"][0], (float)obj["Rect"][1], (float)obj["Rect"][2], (float)obj["Rect"][3]);
				}
			}

			if (obj.contains("HasSprite") && obj["HasSprite"].is_boolean()) {
				object->hasTexture = obj["HasSprite"];
			}
			if (obj.contains("SpriteIndex") && obj["SpriteIndex"].is_number_unsigned()) {
				object->textureIndex = (uint64_t)obj["SpriteIndex"];
			}

			if (obj.contains("Color") && obj["Color"].is_array() && obj["Color"].size() == 4) {
				auto& i = obj["Color"];
				if (i[0].is_number_float() && i[1].is_number_float() && i[2].is_number_float() && i[3].is_number_float()) {
					object->color = Color((float)i[0], (float)i[1], (float)i[2], (float)i[3]);
				}
			}

			if (obj.contains("SpriteRect") && obj["SpriteRect"].is_array() && obj["SpriteRect"].size() == 4) {
				auto& i = obj["SpriteRect"];
				if (i[0].is_number_float() && i[1].is_number_float() && i[2].is_number_float() && i[3].is_number_float()) {
					object->textureRect = FloatRect((float)i[0], (float)i[1], (float)i[2], (float)i[3]);
				}
			}

			if (obj.contains("Colliders") && obj["Colliders"].is_array()) {
				for (auto& col : obj["Colliders"]) {
					Collider collider;

					if (col.contains("Data") && col["Data"].is_array() && col["Data"].size() == 6) {
						auto& i = col["Data"];
						if (i[0].is_number_float() && i[1].is_number_float() && i[2].is_number_float() && i[3].is_number_float() && i[4].is_number_float() && i[5].is_number_float()) {
							collider.triangle.point[0] = vec2((float)i[0], (float)i[1]);
							collider.triangle.point[1] = vec2((float)i[2], (float)i[3]);
							collider.triangle.point[2] = vec2((float)i[4], (float)i[5]);
						}
					}

					if (col.contains("IsTrigger") && col["IsTrigger"].is_boolean()) {
						collider.isTrigger = (bool)col["IsTrigger"];
					}
					if (col.contains("Tag") && col["Tag"].is_string()) {
						std::string tag = col["Tag"];

						collider.tag = Collider::Tags::Default;

						if      (tag == "CameraFree")		collider.tag = Collider::Tags::CameraFree;
						else if (tag == "CameraLocked")		collider.tag = Collider::Tags::CameraLocked;
						else if (tag == "Climb")			collider.tag = Collider::Tags::Climb;
						else if (tag == "Dashable")			collider.tag = Collider::Tags::Dashable;
						else if (tag == "ElevatorScreen")	collider.tag = Collider::Tags::ElevatorScreen;
						else if (tag == "Ground")			collider.tag = Collider::Tags::Ground;
						else if (tag == "GroundCheck")		collider.tag = Collider::Tags::GroundCheck;
						else if (tag == "Interact")			collider.tag = Collider::Tags::Interact;
						else if (tag == "PowerOff")			collider.tag = Collider::Tags::PowerOff;
						else if (tag == "PowerOn")			collider.tag = Collider::Tags::PowerOn;
						else if (tag == "AirStreamNorth")	collider.tag = Collider::Tags::AirStreamNorth;
						else if (tag == "AirStreamEast")	collider.tag = Collider::Tags::AirStreamEast;
						else if (tag == "AirStreamSouth")	collider.tag = Collider::Tags::AirStreamSouth;
						else if (tag == "AirStreamWest")	collider.tag = Collider::Tags::AirStreamWest;
						else if (tag == "Damage")			collider.tag = Collider::Tags::Damage;
						else if (tag == "Death")			collider.tag = Collider::Tags::Death;
						else if (tag == "Light")			collider.tag = Collider::Tags::Light;
						else if (tag == "TorchLight")		collider.tag = Collider::Tags::TorchLight;
						else if (tag == "CheckPoint")		collider.tag = Collider::Tags::CheckPoint;
						else if (tag == "Lane")				collider.tag = Collider::Tags::Lane;
						else if (tag == "Warp")				collider.tag = Collider::Tags::Warp;
					}
					if (col.contains("Type") && col["Type"].is_number_unsigned()) {
						collider.type = (ColliderType)(uint8_t)col["Type"];
					}

					switch (collider.type) {
					case ColliderType::BoxCollider:
						collider.box = ConvertRect(object->transform.rect, collider.box);
						break;
					case ColliderType::CircleCollider:
						collider.circle.center = ConvertPos(object->transform.rect, collider.circle.center);
						collider.circle.radius = ConvertRect(object->transform.rect, FloatRect(0.0f, 0.0f, collider.circle.radius, 0.0f)).w;
						break;
					case ColliderType::TriangleCollider:
						collider.triangle.point[0] = ConvertPos(object->transform.rect, collider.triangle.point[0]);
						collider.triangle.point[1] = ConvertPos(object->transform.rect, collider.triangle.point[1]);
						collider.triangle.point[2] = ConvertPos(object->transform.rect, collider.triangle.point[2]);
						break;
					}

					collider.parent = object.get();

					object->colliders.push_back(collider);
				}
			}

			object->type = ObjectType::Static;

			m_Engine.AddObject(object);
		}
	}
}