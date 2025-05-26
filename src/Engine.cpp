#include "Engine.hpp"

Engine::Engine() {

}

Engine::~Engine() {
	
}

void Engine::AddObject(std::shared_ptr<Object> object) {
	m_Objects.push_back(object);
}

void Engine::Update(Input& input) {
	for (auto obj : m_Objects) {
		if (obj->exists)
			obj->ColliderUpdate(m_Objects);
	}
	for (auto obj : m_Objects) {
		if (obj->exists)
			obj->Update(input);
	}
	for (auto obj : m_Objects) {
		if (!obj->exists) continue;
		for (auto obj2 : m_Objects) {
			if (!obj->exists) break;
			if (!obj2->exists) continue;
			if (obj != obj2) {
				obj->Collide(*obj2);
			}
		}
	}
}